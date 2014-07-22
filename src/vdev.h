#ifndef _NES_VDEV_H_
#define _NES_VDEV_H_

// ����ͷ�ļ�
#include "stdefine.h"

// ��������
void* vdev_create (int w, int h, int depth, DWORD extra);
void  vdev_destroy(void *ctxt);
void  vdev_setpal (void *ctxt, int i, int n, BYTE *pal);
void  vdev_getpal (void *ctxt, int i, int n, BYTE *pal);
void  vdev_lock   (void *ctxt, void **buf, int *stride);
void  vdev_unlock (void *ctxt);

#endif
