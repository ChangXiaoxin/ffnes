// ����ͷ�ļ�
#include "vdev.h"

typedef struct
{
    int     width;      /* ��� */
    int     height;     /* �߶� */
    int     cdepth;     /* ����λ��� */
    int     stride;     /* �п��ֽ��� */
    void   *pdata;      /* ָ��ͼ������ */
    BYTE   *ppal;       /* ָ��ɫ������ */
    HWND    hwnd;       /* ���ھ�� */
    HDC     hdcsrc;
    HDC     hdcdst;
    HBITMAP hbmp;
} VDEV;

// �ڲ�����ʵ��
static void createstdpal(BYTE *pal)
{
    int r, g, b;
    for (r=0; r<256; r+=36)
    {
        for (g=0; g<256; g+=36)
        {
            for (b=0; b<256; b+=85)
            {
                *pal++ = b;
                *pal++ = g;
                *pal++ = r;
                *pal++ = 0;
            }
        }
    }
}

// ����ʵ��
void* vdev_create(int w, int h, int depth, DWORD extra)
{
    VDEV *dev = malloc(sizeof(VDEV));
    if (dev)
    {
        BITMAP      bitmap  = {0};
        BYTE        buffer[sizeof(BITMAPINFOHEADER) + sizeof(DWORD) * 256] = {0};
        BITMAPINFO *bmpinfo = (BITMAPINFO*)buffer;

        bmpinfo->bmiHeader.biSize        =  sizeof(BITMAPINFOHEADER);
        bmpinfo->bmiHeader.biWidth       =  w;
        bmpinfo->bmiHeader.biHeight      = -h;
        bmpinfo->bmiHeader.biPlanes      =  1;
        bmpinfo->bmiHeader.biBitCount    =  depth;
        bmpinfo->bmiHeader.biCompression =  BI_RGB;

        switch (depth)
        {
        case 8:
            createstdpal((BYTE*)bmpinfo->bmiColors);
            break;

        case 16:
            bmpinfo->bmiHeader.biCompression = BI_BITFIELDS;
            ((DWORD*)bmpinfo->bmiColors)[0]  = 0xF800;
            ((DWORD*)bmpinfo->bmiColors)[1]  = 0x07E0;
            ((DWORD*)bmpinfo->bmiColors)[2]  = 0x001F;
            break;
        }

        dev->width  = w;
        dev->height = h;
        dev->cdepth = depth;
        dev->hwnd   = (HWND)extra;
        dev->hdcdst = GetDC(dev->hwnd);
        dev->hdcsrc = CreateCompatibleDC(dev->hdcdst);
        dev->hbmp   = CreateDIBSection(dev->hdcsrc, bmpinfo, DIB_RGB_COLORS, (void**)&(dev->pdata), NULL, 0);

        GetObject(dev->hbmp, sizeof(BITMAP), &bitmap);
        dev->stride = bitmap.bmWidthBytes;
        SelectObject(dev->hdcsrc, dev->hbmp);
    }
    return dev;
}

void vdev_destroy(void *ctxt)
{
    VDEV *dev = (VDEV*)ctxt;
    if (dev == NULL) return;
    if (dev->hdcsrc) DeleteDC (dev->hdcsrc);
    if (dev->hdcdst) ReleaseDC(dev->hwnd, dev->hdcdst);
    if (dev->hbmp  ) DeleteObject(dev->hbmp);
    free(dev);
}

void vdev_setpal(void *ctxt, int i, int n, BYTE *pal)
{
    VDEV *dev = (VDEV*)ctxt;
    if (dev == NULL) return;
    SetDIBColorTable(dev->hdcsrc, i, n, (RGBQUAD*)pal);
}

void vdev_getpal(void *ctxt, int i, int n, BYTE *pal)
{
    VDEV *dev = (VDEV*)ctxt;
    if (dev == NULL) return;
    GetDIBColorTable(dev->hdcsrc, i, n, (RGBQUAD*)pal);
}

void vdev_lock(void *ctxt, void **buf, int *stride)
{
    VDEV *dev = (VDEV*)ctxt;
    if (dev == NULL) return;
    if (buf   ) *buf    = dev->pdata;
    if (stride) *stride = dev->stride;
}

void vdev_unlock(void *ctxt)
{
    RECT rect = {0};
    VDEV *dev = (VDEV*)ctxt;
    if (dev == NULL) return;

    GetClientRect(dev->hwnd, &rect);
    StretchBlt(dev->hdcdst, rect.left, rect.top, rect.right, rect.bottom,
               dev->hdcsrc, 0, 0, dev->width, dev->height, SRCCOPY);
}
