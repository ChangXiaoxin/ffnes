#ifndef _NES_CPU_H_
#define _NES_CPU_H_

// ����ͷ�ļ�
#include "stdefine.h"
#include "bus.h"

// ��������
#define NMI_VECTOR  0xfffa
#define RST_VECTOR  0xfffc
#define IRQ_VECTOR  0xfffe

// ���Ͷ���
typedef struct {
    WORD pc;
    BYTE sp;
    BYTE ax;
    BYTE xi;
    BYTE yi;
    BYTE ps;
    BUS  cbus;       // cpu bus
    BYTE cram[2048]; // cpu ram, 2KB

// private:
    int nmi_last;
    int nmi_cur;
    int irq_flag;
    int pclk_divider;
    int cclk_counter;
    int cclk_instr;
    int cclk_dma;
} CPU;

// ��������
void cpu_init    (CPU *cpu, BUS cbus);
void cpu_free    (CPU *cpu);
void cpu_reset   (CPU *cpu);
void cpu_nmi     (CPU *cpu, int nmi);
void cpu_irq     (CPU *cpu, int irq);
void cpu_run_pclk(CPU *cpu);

#endif

