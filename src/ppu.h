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
    int  _2005_toggle;
    int  _2006_toggle;
    WORD  ppu_addr;
    WORD  ppu_tmp0;
    WORD  ppu_tmp1;
    int   color_flags;
    BYTE *draw_buffer;
    int   draw_stride;
    BYTE *chrom_bkg;
    BYTE *chrom_spr;
} PPU;

// ��������
void ppu_init  (PPU *ppu, DWORD extra);
void ppu_free  (PPU *ppu);
void ppu_reset (PPU *ppu);
void ppu_run   (PPU *ppu, int scanline);

BYTE NES_PPU_REG_RCB(MEM *pm, int addr);
void NES_PPU_REG_WCB(MEM *pm, int addr, BYTE byte);

#endif

