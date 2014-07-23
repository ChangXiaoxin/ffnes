#ifndef _NES_CPU_H_
#define _NES_CPU_H_

// ����ͷ�ļ�
#include "stdefine.h"
#include "bus.h"

// ���Ͷ���
typedef struct {
    WORD pc;
    BYTE sp;
    BYTE ax;
    BYTE xi;
    BYTE yi;
    BYTE ps;
    int  nmi_last;
    int  nmi_cur;
    long cycles_emu;
    long cycles_real;

// private:
    BUS   cbus; // cpu bus
    BYTE *cram; // cpu ram
} CPU;

// ��������
void cpu_init (CPU *cpu, BUS cbus);
void cpu_reset(CPU *cpu);
void cpu_nmi  (CPU *cpu, int nmi);
void cpu_run  (CPU *cpu, int cycle);

#endif

