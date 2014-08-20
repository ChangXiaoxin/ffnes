#ifndef _NES_BUS_H_
#define _NES_BUS_H_

// ����ͷ�ļ�
#include "stdefine.h"
#include "mem.h"

// ��������
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
} *BUS, BUSITEM;

// ��������
BYTE bus_readb (BUS bus, int baddr);
WORD bus_readw (BUS bus, int baddr);
void bus_writeb(BUS bus, int baddr, BYTE  byte);
void bus_writew(BUS bus, int baddr, WORD  word);
void bus_setmem(BUS bus, int i, int start, int end, MEM *membank);
void bus_setmir(BUS bus, int i, int start, int end, WORD mirmask);

#endif



