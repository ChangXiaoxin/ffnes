#ifndef _NES_MEM_H_
#define _NES_MEM_H_

// ����ͷ�ļ�
#include "stdefine.h"

// ���Ͷ���
enum {
    MEM_ROM,
    MEM_RAM,
    MEM_REG,
};

typedef struct tagMEM {
    int   type;
    int   size;
    BYTE *data;
    void (*r_callback)(struct tagMEM *pm, int addr);
    void (*w_callback)(struct tagMEM *pm, int addr);
} MEM;

// ��������
BYTE mem_readb (MEM *pm, int addr);
WORD mem_readw (MEM *pm, int addr);
void mem_writeb(MEM *pm, int addr, BYTE  byte);
void mem_writew(MEM *pm, int addr, WORD  word);

#endif

