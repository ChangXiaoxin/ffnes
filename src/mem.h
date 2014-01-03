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

typedef struct {
    int   type;
    int   size;
    BYTE *data;
} MEM;

// ��������
void mem_read (MEM *pm, int addr, BYTE *byte);
void mem_write(MEM *pm, int addr, BYTE  byte);

#endif

