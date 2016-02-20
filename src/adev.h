#ifndef _NES_ADEV_H_
#define _NES_ADEV_H_

// ����ͷ�ļ�
#include "stdefine.h"

// ���Ͷ���
typedef struct {
    BYTE  *lpdata;
    DWORD  buflen;
} AUDIOBUF;

typedef struct {
    void* (*create    )(int bufnum, int buflen);
    void  (*destroy   )(void *ctxt);
    void  (*bufrequest)(void *ctxt, AUDIOBUF **ppab);
    void  (*bufpost   )(void *ctxt, AUDIOBUF  * pab);
} ADEV;

// ȫ�ֱ�������
extern ADEV DEV_WAVEOUT;

#endif
