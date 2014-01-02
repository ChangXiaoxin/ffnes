// ����ͷ�ļ�
#include <stdlib.h>
#include "mem.h"

// ����ʵ��
BOOL mem_create(MEM *pm)
{
    pm->data = malloc(pm->size);
    if (!pm->data) return FALSE;
    else return TRUE;
}

void mem_destroy(MEM *pm)
{
    if (pm->data) {
        free(pm->data);
        pm->data = NULL;
    }
}

void mem_read(MEM *pm, int addr, BYTE *byte)
{
    *byte = pm->data[addr % pm->size];
}

void mem_write(MEM *pm, int addr, BYTE byte)
{
    if (pm->type != MEM_ROM) {
        pm->data[addr % pm->size] = byte;
    }
}

