#ifndef _NES_CPU_H_
#define _NES_CPU_H_

// ����ͷ�ļ�
#include "stdefine.h"

// ���Ͷ���
typedef struct {
    BYTE pc;
    BYTE sp;
    BYTE ax;
    BYTE xi;
    BYTE yi;
    BYTE ps;
    long cycles;
} CPU;

void cpu_reset(CPU *cpu);

#endif

