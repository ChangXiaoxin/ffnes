/* ��׼ͷ�ļ� */
#ifndef _STDEFINE_H_
#define _STDEFINE_H_

#if defined(__MINGW32__)
#include <windows.h>
#undef RGB // undef RGB
#else

/* �������� */
#define TRUE   1
#define FALSE  0

/* ��׼�����Ͷ��� */
typedef int BOOL;
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef long     int   LONG;

#endif

/* �ú�������������δʹ�õľ��� */
#define DO_USE_VAR(var)  do { var = var; } while (0)

#endif


