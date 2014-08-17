#ifndef _NES_PPU_H_
#define _NES_PPU_H_

// ����ͷ�ļ�
#include "stdefine.h"
#include "mem.h"

// ���Ͷ���
typedef struct {
    BYTE  regs   [8  ]; // ppu regs
    BYTE  sprram [256]; // sprite ram
    BYTE  palette[32 ]; // palette

// private:
    void *vdevctxt;
    int   pin_vbl;
    int   toggle;
    WORD  vaddr;
    WORD  temp0;
    WORD  temp1;
    int   color_flags;
    BYTE *draw_buffer;
    int   draw_stride;
    BYTE *chrom_bkg;
    BYTE *chrom_spr;
    int   scanline;
} PPU;

// ��������
void ppu_init  (PPU *ppu, DWORD extra);
void ppu_free  (PPU *ppu);
void ppu_reset (PPU *ppu);
void ppu_run   (PPU *ppu, int scanline);

BYTE NES_PPU_REG_RCB(MEM *pm, int addr);
void NES_PPU_REG_WCB(MEM *pm, int addr, BYTE byte);

#endif

