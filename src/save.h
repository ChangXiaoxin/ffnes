#ifndef _NES_SAVE_H_
#define _NES_SAVE_H_

// ����ͷ�ļ�
#include "nes.h"

// Ԥ���뿪��
#define USE_LZW_COMPRESS   1

// ��������
void saver_save_game  (NES *nes, char *file);
void saver_load_game  (NES *nes, char *file);
void saver_load_replay(NES *nes, char *file);

#endif

