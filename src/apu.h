#ifndef _NES_APU_H_
#define _NES_APU_H_

// ����ͷ�ļ�
#include "stdefine.h"
#include "adev.h"
#include "mem.h"

// ���Ͷ���
typedef struct {
    BYTE  regs[0x16];

// private:
    void     *adevctxt;
    AUDIOBUF *audiobuf;
    long      pclk_frame;
} APU;

// ��������
void apu_init (APU *apu, DWORD extra);
void apu_free (APU *apu);
void apu_reset(APU *apu);
void apu_run_pclk(APU *apu, int pclk);

BYTE NES_APU_REG_RCB(MEM *pm, int addr);
void NES_APU_REG_WCB(MEM *pm, int addr, BYTE byte);

#endif

