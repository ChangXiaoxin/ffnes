#ifndef _NES_BUS_H_
#define _NES_BUS_H_

// ����ͷ�ļ�
#include "stdefine.h"
#include "mem.h"

// ��������
#define NES_MAX_BUS_SIZE  16

enum {
    BUS_MAP_MEMORY,
    BUS_MAP_MIRROR,
};

// ���Ͷ���
typedef struct {
    int  type;
    int  start;
    int  end;
    union {
        MEM *membank;
        WORD mirmask;
    };
} BUS[NES_MAX_BUS_SIZE];

// ��������
void bus_read  (BUS bus, int baddr, BYTE *data);
void bus_write (BUS bus, int baddr, BYTE  data);
void bus_setmem(BUS bus, int i, int start, int end, MEM *membank);
void bus_setmir(BUS bus, int i, int start, int end, WORD mirmask);

#endif



