#ifndef _NES_APU_H_
#define _NES_APU_H_

// ����ͷ�ļ�
#include "stdefine.h"

// ���Ͷ���
typedef struct {
    void *adevctxt;
    BYTE  reserved;
} APU;

// ��������
void apu_init (APU *apu, DWORD extra);
void apu_free (APU *apu);
void apu_reset(APU *apu);
void apu_render_frame(APU *apu);

#endif

