#ifndef _NES_ADEV_H_
#define _NES_ADEV_H_

// ����ͷ�ļ�
#include "stdefine.h"

// ���Ͷ���
typedef struct {
    BYTE  *lpdata;
    DWORD  buflen;
} AUDIOBUF;

// ��������
void* adev_create (int bufnum, int buflen);
void  adev_destroy(void *ctxt);
void  adev_buf_request(void *ctxt, AUDIOBUF **ppab);
void  adev_buf_post   (void *ctxt, AUDIOBUF  * pab);

#endif
