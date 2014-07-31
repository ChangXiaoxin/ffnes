// ����ͷ�ļ�
#include "mem.h"

// ����ʵ��
BYTE mem_readb(MEM *pm, int addr)
{
    // memory read callback
    if (pm->r_callback) pm->r_callback(pm, addr);

    if (pm->data) return pm->data[addr % pm->size];
    else return 0;
}

WORD mem_readw(MEM *pm, int addr)
{
    // memory read callback
    if (pm->r_callback) {
        pm->r_callback(pm, addr + 0);
        pm->r_callback(pm, addr + 1);
    }

    if (pm->data) return *(WORD*)(pm->data + addr % pm->size);
    else return 0;
}

void mem_writeb(MEM *pm, int addr, BYTE byte)
{
    if (pm->type != MEM_ROM) {
        if (pm->data) pm->data[addr % pm->size] = byte;
    }

    // memory write callback
    if (pm->w_callback) pm->w_callback(pm, addr, byte);
}

void mem_writew(MEM *pm, int addr, WORD word)
{
    if (pm->type != MEM_ROM) {
        if (pm->data) *(WORD*)(pm->data + addr % pm->size) = word;
    }

    // memory write callback
    if (pm->w_callback) {
        pm->w_callback(pm, addr + 0, (BYTE)(word > 0));
        pm->w_callback(pm, addr + 1, (BYTE)(word > 1));
    }
}

