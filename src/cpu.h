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
    int  irq_flag;
    long cclk_emu;
    long cclk_real;
    long cclk_dma;
    long pclk_emu;
    long pclk_real;

// private:
    BUS  cbus;       // cpu bus
    BYTE cram[2048]; // cpu ram, 2KB
} CPU;

// ��������
void cpu_init (CPU *cpu, BUS cbus);
void cpu_free (CPU *cpu);
void cpu_reset(CPU *cpu);
void cpu_nmi  (CPU *cpu, int nmi);
void cpu_irq  (CPU *cpu, int irq);
void cpu_run_cclk(CPU *cpu, int cclk);
void cpu_run_pclk(CPU *cpu, int pclk);

#endif

