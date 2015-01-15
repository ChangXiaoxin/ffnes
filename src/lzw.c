// ����ͷ�ļ�
#include "lzw.h"
#include "log.h"

/* ���Ͷ��� */
/* LZW ���������Ͷ��� */
/* prefixe code -1 means null */
typedef struct
{
    int  prefix;    /* ǰ׺       */
    int  firstbyte; /* ���ֽ�     */
    int  lastbyte;  /* β�ֽ�     */
    int  head;      /* ǰ׺��ͷ   */
    int  next;      /* ǰ׺��ָ�� */
} LZWSTRITEM, *PLZWSTRITEM;

/* LZW ����������Ͷ��� */
typedef struct
{
    #define LZW_TAB_SIZE       4096
    #define LZW_ROOT_SIZE      8
    #define LZW_CODE_SIZE_OUT  8
    #define LZW_CODE_SIZE_MIN  9
    #define LZW_CODE_SIZE_MAX  12
    #define LZW_ROOT_NUM       256
    #define LZW_CLR_CODE       256
    #define LZW_END_CODE       257

    #define LZW_MODE_DECODE    0
    #define LZW_MODE_ENCODE    1
    int   mode;
    FILE *fp  ;
    BYTE  bitbuf;
    int   bitflag;

    /* �ڲ�ʹ�õı��� */
    LZWSTRITEM lzw_str_tab[LZW_TAB_SIZE];
    int        str_tab_pos;
    int        lzw_str_buf[LZW_TAB_SIZE];
    int        str_buf_pos;
    int        curcodesize;
    int        prefixcode;
    int        oldcode;

    // for lzw_fseek & lzw_ftell
    long       fpos;
} LZW;

// �ڲ�����ʵ��
static void resetlzwcodec(LZW *lzw)
{
    if (lzw->mode == LZW_MODE_ENCODE)
    {
        LZWSTRITEM *strtab_cur = &(lzw->lzw_str_tab[0]);
        LZWSTRITEM *strtab_end = strtab_cur + LZW_TAB_SIZE;
        /* init lzw prefix link list */
        do { strtab_cur->head = 0; } while (++strtab_cur < strtab_end);
    }

    /* reset */
    lzw->str_tab_pos = LZW_END_CODE + 1;
    lzw->curcodesize = LZW_CODE_SIZE_MIN;
    lzw->prefixcode  = -1;
    lzw->oldcode     = -1;
}

/* �� LZW ������м���һ���ַ��� */
static BOOL AddToLZWStringTable(LZW *lzw, int prefix, int byte, BOOL flag)
{
    LZWSTRITEM *strtab = (LZWSTRITEM*)lzw->lzw_str_tab;

    /* ǰ׺Ϊ�յ��ַ���ֱ�ӷ��سɹ� */
    if (prefix == -1) return TRUE;

    /* ��� LZW ��������򷵻�ʧ�� */
    if (lzw->str_tab_pos >= LZW_TAB_SIZE) return FALSE;

    /* �ڱ�����м������ַ��� */
    strtab[lzw->str_tab_pos].prefix    = prefix;
    strtab[lzw->str_tab_pos].firstbyte = strtab[prefix].firstbyte;
    strtab[lzw->str_tab_pos].lastbyte  = byte;

    if (flag)
    {
        /* ����ǰ׺���� */
        strtab[lzw->str_tab_pos].next = strtab[prefix].head;
        strtab[prefix].head           = lzw->str_tab_pos;
    }

    /* ���ӱ����ǰλ��ָ�� */
    lzw->str_tab_pos++;

    /* ���سɹ� */
    return TRUE;
}

/*
  �ڱ�����в���һ���ַ����ı���
  �ҵ��򷵻�����룬���򷵻� -1 .
 */
static int FindInLZWStringTable(LZW *lzw, int prefix, int byte)
{
    LZWSTRITEM *strtab = (LZWSTRITEM*)lzw->lzw_str_tab;
    int         link;

    /* ���ǰ׺Ϊ -1 ��ֱ�ӷ��� */
    if (prefix == -1) return byte;

    /* �� while ѭ���б���ǰ׺���� */
    link = strtab[prefix].head;
    while (link)
    {
        if (strtab[link].lastbyte == byte) return link;
        link = strtab[link].next;
    }

    /* û���ҵ��򷵻� -1 */
    return -1;
}

/* �жϵ�ǰ��������Ƿ��Ѿ�����ĳ������ */
static BOOL IsCodeInLZWStringTable(LZW *lzw, int code)
{
    if (code >= 0 && code < lzw->str_tab_pos) return TRUE;
    else return FALSE;
}

/* ȡ��һ�������Ӧ�ַ��������ַ� */
static int GetFirstByteOfLZWCode(LZW *lzw, int code)
{
    return ((LZWSTRITEM*)lzw->lzw_str_tab)[code].firstbyte;
}

/* ���� lzw �ַ�������� lzw_str_buf */
static void DecodeLZWString(LZW *lzw, int code)
{
    LZWSTRITEM *strtab = (LZWSTRITEM*)lzw->lzw_str_tab;
    int        *pbuf   = lzw->lzw_str_buf;

    /* ���� LZW �ַ��� */
    while (code != -1 && lzw->str_buf_pos++ < LZW_TAB_SIZE)
    {
       *pbuf++ = strtab[code].lastbyte;
        code   = strtab[code].prefix;
    }
}

BOOL getbits(LZW *lzw, DWORD *data, int size)
{
    int pos, byte, need, i;

    need = (size - lzw->bitflag + 7) / 8;
    if (need > 0)
    {
       *data = lzw->bitbuf;
        pos  = lzw->bitflag;
        for (i=0; i<need; i++) {
            byte = fgetc(lzw->fp);
            if (byte == EOF) return FALSE;

           *data |= byte << pos;
            pos  += 8;
        }
        lzw->bitflag = need * 8 + lzw->bitflag - size;
        lzw->bitbuf  = byte >> (8 - lzw->bitflag);
    }
    else
    {
       *data  = lzw->bitbuf;
        lzw->bitflag -= size;
        lzw->bitbuf >>= size;
    }
    *data &= (1L << size) - 1;
    return TRUE;
}

BOOL putbits(LZW *lzw, DWORD data, int size)
{
    int nbit;

    data &= (1L << size) - 1;

    if (lzw->bitflag)
    {
        nbit = size < 8 - lzw->bitflag ? size : 8 - lzw->bitflag;
        lzw->bitbuf |= data << lzw->bitflag;
        lzw->bitflag+= nbit;
        if (lzw->bitflag < 8) return TRUE;

        if (EOF == fputc(lzw->bitbuf, lzw->fp)) return FALSE;
        data >>= nbit;
        size  -= nbit;
    }

    while (size >= 8)
    {
        if (EOF == fputc(data, lzw->fp)) return FALSE;
        data >>= 8;
        size  -= 8;
    }

    lzw->bitflag = size;
    lzw->bitbuf  = (BYTE)data;
    return TRUE;
}

BOOL flushbits(LZW *lzw, int flag)
{
    DWORD fill;

    if (!lzw->bitflag) return TRUE;

    if (flag) fill = 0xffffffff;
    else      fill = 0;

    if (!putbits(lzw, fill, 8 - lzw->bitflag)) return FALSE;
    else return TRUE;
}

// ����ʵ��
void* lzw_fopen(const char *filename, const char *mode)
{
    LZW        *lzw    = NULL;
    LZWSTRITEM *strtab = NULL;
    int         i;

    // allocate lzw context
    lzw = malloc(sizeof(LZW));
    if (!lzw) return NULL;

    // init lzw context
    memset(lzw, 0, sizeof(LZW));
    lzw->mode = *mode == 'w' ? LZW_MODE_ENCODE : LZW_MODE_DECODE;
    lzw->fp   = fopen(filename, mode);

    // init lzw root code
    strtab = lzw->lzw_str_tab;
    for (i=0; i<LZW_END_CODE; i++)
    {
        strtab[i].prefix    = -1;
        strtab[i].firstbyte =  i;
        strtab[i].lastbyte  =  i;
    }

    // reset lzw codec
    resetlzwcodec(lzw);

    return lzw;
}

int lzw_fclose(void *stream)
{
    LZW *lzw = (LZW*)stream;
    if (lzw)
    {
        if (lzw->fp)
        {
            if (!putbits(lzw, lzw->prefixcode, lzw->curcodesize)) return EOF;
            if (!putbits(lzw, LZW_END_CODE   , lzw->curcodesize)) return EOF;
            if (!flushbits(lzw, 0)) return EOF;
            fclose(lzw->fp);
        }
        free(lzw);
    }
    return 0;
}

int lzw_fgetc(void *stream)
{
    LZW  *lzw     = (LZW*)stream;
    DWORD curcode = 0;
    BOOL  find    = FALSE;

    if (lzw->mode == LZW_MODE_ENCODE) return EOF;

    /* ��һ�� while ѭ���н��н���
       ÿ�ζ��� LZW ���뵽 curcode ��
       Ȼ����� LZW �Ľ��봦��
       ֱ������ LZW_END_CODE ���� */
    while (  lzw->str_buf_pos == 0
          && getbits(lzw, &curcode, lzw->curcodesize)
          && curcode != LZW_END_CODE )
    {
        if (curcode == LZW_CLR_CODE)
        {   /* ���������������� */
            resetlzwcodec(lzw); /* reset lzw codec */
        }
        else
        {   /* ��������Ĳ�������� */
            /* ���ұ�������Ƿ��иñ��� */
            find = IsCodeInLZWStringTable(lzw, curcode);

            /* ���ݲ��ҽ���������м����µ��ַ��� */
            if (find) AddToLZWStringTable(lzw, lzw->oldcode, GetFirstByteOfLZWCode(lzw, curcode), FALSE);
            else AddToLZWStringTable(lzw, lzw->oldcode, GetFirstByteOfLZWCode(lzw, lzw->oldcode), FALSE);

            /* ���� _str_tab_pos ���¼����µ� curcodesize */
            if (   lzw->str_tab_pos == (1 << lzw->curcodesize)
                && lzw->curcodesize < LZW_CODE_SIZE_MAX) // note: ���������������Ҫ
            {
                /* ���� curcodesize ��ֵ */
                lzw->curcodesize++;
            }

            /* ���뵱ǰ�ַ��� */
            DecodeLZWString(lzw, curcode);

            /* oldcode ��Ϊ curcode */
            lzw->oldcode = curcode;
        }
    }

    if (lzw->str_buf_pos <= 0) return EOF;
    else
    {
        lzw->fpos++; // file read/write pos
        return lzw->lzw_str_buf[--lzw->str_buf_pos];
    }
}

int lzw_fputc(int c, void *stream)
{
    LZW *lzw  = (LZW*)stream;
    int  find = -1;

    if (lzw->mode == LZW_MODE_DECODE) return EOF;

    /* �� LZW ������в����ɵ�ǰǰ׺�͵�ǰ�ַ���ɵ��ַ��� */
    find = FindInLZWStringTable(lzw, lzw->prefixcode, c);
    if (find == -1)
    {   /* �� LZW �������û�и��ַ��� */
        /* �Ե�ǰ�� codesize д����ǰǰ׺ */
        if (!putbits(lzw, lzw->prefixcode, lzw->curcodesize)) {
            log_printf("failed to write prefixcode !\n");
            return EOF;
        }

        /* ����ǰǰ׺�͵�ǰ�ַ�����ɵ��ַ����������� */
        if (!AddToLZWStringTable(lzw, lzw->prefixcode, c, TRUE))
        {   /* ����ʧ��˵����������� */
            /* д��һ�� LZW_CLEAR_CODE �������� */
            if (!putbits(lzw, LZW_CLR_CODE, lzw->curcodesize)) {
                log_printf("failed to write LZW_CLEAR_CODE !\n");
                return EOF;
            }
            resetlzwcodec(lzw); /* reset lzw codec */
        }
        else 
        {   /* ���뵽�����ɹ� */
            /* ���� _str_tab_pos ���¼����µ� curcodesize */
            if (   lzw->str_tab_pos - 1 == (1 << lzw->curcodesize)
                && lzw->curcodesize < LZW_CODE_SIZE_MAX) /* note: ���������������Ҫ */
            {
                /* ���� curcodesize ��ֵ */
                lzw->curcodesize++;
            }
        }

        /* �õ�ǰǰ׺��Ϊ currentbyte */
        lzw->prefixcode = c;
    }
    else
    {   /* �� LZW ��������ҵ����ַ��� */
        /* �õ�ǰǰ׺��Ϊ���ַ����ı��� */
        lzw->prefixcode = find;
    }

    // file read/write pos
    lzw->fpos++;
    return 0;
}

size_t lzw_fread(void *buffer, size_t size, size_t count, void *stream)
{
    BYTE  *dst   = (BYTE*)buffer;
    size_t total = size * count;
    while (total--)
    {
        *dst = lzw_fgetc(stream);
        if (*dst++ == EOF)
        {
            total++;
            break;
        }
    }
    return (size * count - total);
}

size_t lzw_fwrite(void *buffer, size_t size, size_t count, void *stream)
{
    BYTE  *dst   = (BYTE*)buffer;
    size_t total = size * count;
    while (total--)
    {
        if (EOF == lzw_fputc(*dst++, stream))
        {
            total++;
            break;
        }
    }
    return (size * count - total);
}

int lzw_fseek(void *stream, long offset, int origin)
{
    LZW *lzw  = (LZW*)stream;
    int  skip = 0;

    // don't support seek for lzw encode mode
    if (lzw->mode == LZW_MODE_ENCODE) return EOF;

    // don't support seek if origin is SEEK_END
    if (origin == SEEK_END) return EOF;

    switch (origin)
    {
    case SEEK_SET: offset = offset; break;
    case SEEK_CUR: offset = lzw->fpos + offset; break;
    }

    // already at the offset, return directly
    if (offset == lzw->fpos) return offset;

    // seek lzw file
    skip = offset - lzw->fpos;
    if (skip < 0)
    {
        resetlzwcodec(lzw);
        lzw->str_buf_pos = 0;
        lzw->fpos        = 0;
        fseek(lzw->fp, 0, SEEK_SET);
        skip = offset;
    }
    while (skip--) lzw_fgetc(stream);

    return 0;
}

long lzw_ftell(void *stream) { return ((LZW*)stream)->fpos; }


