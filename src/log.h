#ifndef _NES_LOG_H_
#define _NES_LOG_H_

// ����ͷ�ļ�
#include "stdefine.h"

/* �������� */
void log_init  (const char *file);
void log_done  (void);
void log_printf(const char *format, ...);

#endif
