#ifndef _NES_SAVE_H_
#define _NES_SAVE_H_

// ����ͷ�ļ�
#include <stdio.h>

// ��������
enum {
    NES_REPLAY_RECORD,
    NES_REPLAY_PLAY  ,
};

// ���Ͷ���
typedef struct {
    int   mode;
    FILE *fp;
    long  total;
    long  curpos;
    char  fname[MAX_PATH];
} REPLAY;

// ��������
void replay_init (REPLAY *rep);
void replay_free (REPLAY *rep);
void replay_reset(REPLAY *rep);
BYTE replay_run  (REPLAY *rep, BYTE data);
int  replay_isend(REPLAY *rep);

void save_game  (void *nes, char *file);
void load_game  (void *nes, char *file);
void load_replay(void *nes, char *file);

#endif

