#ifndef _NES_LZW_H_
#define _NES_LZW_H_

// ����ͷ�ļ�
#include <stdio.h>
#include "stdefine.h"

// ��������
void* lzw_fopen (const char *filename, const char *mode);
int   lzw_fclose(void *stream);
int   lzw_fgetc (void *stream);
int   lzw_fputc (int c, void *stream);

#endif
