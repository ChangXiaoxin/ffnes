#ifndef _NES_APU_H_
#define _NES_APU_H_

// ����ͷ�ļ�
#include "stdefine.h"
#include "mem.h"

// ���Ͷ���
typedef struct {
    BYTE  regs[22];

// private:
    void *adevctxt;
} APU;

// ��������
void apu_init (APU *apu, DWORD extra);
void apu_free (APU *apu);
void apu_reset(APU *apu);
void apu_render_frame(APU *apu);

BYTE NES_APU_REG_RCB(MEM *pm, int addr);
void NES_APU_REG_WCB(MEM *pm, int addr, BYTE byte);

#endif

