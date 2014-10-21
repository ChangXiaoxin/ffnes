#ifndef _NES_REPLAY_H_
#define _NES_REPLAY_H_

// ����ͷ�ļ�
#include <stdio.h>
#include "stdefine.h"

// ��������
enum
{
    NES_REPLAY_NONE,
    NES_REPLAY_RECORD,
    NES_REPLAY_PLAY,
};

// ���Ͷ���
typedef struct {
    char  file[MAX_PATH];
    int   mode;
    void *lzwfp;
} REPLAY;

// ��������
void replay_init (REPLAY *rep, char *file, int mode);
void replay_free (REPLAY *rep);
void replay_reset(REPLAY *rep);
BYTE replay_run  (REPLAY *rep, BYTE data);

#endif


