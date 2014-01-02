#ifndef _NES_BUS_H_
#define _NES_BUS_H_

// ����ͷ�ļ�
#include "stdefine.h"
#include "mem.h"

// ��������
#define NES_MAX_BUS_SIZE  8

enum {
    BUS_MAP_MEMORY,
    BUS_MAP_MIRROR,
};

// ���Ͷ���
typedef struct {
    int  type;
    int  start;
    int  end;
    MEM *mem;
} BUS[NES_MAX_BUS_SIZE];

// ��������
void bus_read  (BUS bus, int addr, BYTE *data);
void bus_write (BUS bus, int addr, BYTE  data);
void bus_setmap(BUS bus, int i, int start, int end, MEM *mem);

#endif



