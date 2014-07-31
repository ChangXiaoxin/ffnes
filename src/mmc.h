#ifndef _NES_MMC_H_
#define _NES_MMC_H_

// ����ͷ�ļ�
#include "stdefine.h"
#include "cartridge.h"
#include "bus.h"

// ���Ͷ���
typedef struct {
    CARTRIDGE *cart;
    BUS        cbus;
    BUS        pbus;
    int      number;
    BYTE      *pram;
} MMC;

// ��������
void mmc_init (MMC *mmc, CARTRIDGE *cart, BUS cbus, BUS pbus);
void mmc_free (MMC *mmc);
void mmc_reset(MMC *mmc);

#endif

