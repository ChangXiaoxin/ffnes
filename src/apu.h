#ifndef _NES_APU_H_
#define _NES_APU_H_

// ����ͷ�ļ�
#include "stdefine.h"

// ���Ͷ���
typedef struct {
    BYTE reserved;
} APU;

// ��������
void apu_init (APU *apu);
void apu_reset(APU *apu);

#endif

