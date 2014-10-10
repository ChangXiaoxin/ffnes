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
    BYTE (*r_callback)(struct tagMEM *pm, int addr);
    void (*w_callback)(struct tagMEM *pm, int addr, BYTE data);
} MEM;

// ��������
BYTE mem_readb (MEM *pm, int addr);
WORD mem_readw (MEM *pm, int addr);
void mem_writeb(MEM *pm, int addr, BYTE byte);
void mem_writew(MEM *pm, int addr, WORD word);

// mem read without rw callback
BYTE mem_readb_norwcb(MEM *pm, int addr);
WORD mem_readw_norwcb(MEM *pm, int addr);

#endif

