#ifndef _NES_REPLAY_H_
#define _NES_REPLAY_H_

// ����ͷ�ļ�
#include <stdio.h>
#include "stdefine.h"

// ��������
enum
{
    NES_REPLAY_RECORD,
    NES_REPLAY_PLAY  ,
};

// ���Ͷ���
typedef struct {
    int   mode;
    FILE *fp;
    long  total;
    long  curpos;
} REPLAY;

// ��������
void replay_init    (REPLAY *rep);
void replay_free    (REPLAY *rep);
void replay_reset   (REPLAY *rep);
BYTE replay_run     (REPLAY *rep, BYTE data);
int  replay_progress(REPLAY *rep);

#endif


