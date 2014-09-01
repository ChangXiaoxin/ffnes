#ifndef _NES_NDB_H_
#define _NES_NDB_H_

// 包含头文件
#include "stdefine.h"
#include "cpu.h"

// 常量定义
enum {
    NDB_CPU_KEEP_RUNNING, // cpu keep running
    NDB_CPU_RUN_NSTEPS,   // cpu run n instructions then stop
    NDB_CPU_STOP_PCEQU,   // cpu keep running, but when pc == xxxx will stop
};

// 类型定义
typedef struct
{
    CPU  *cpu;
    int   stop;
    int   cond;
    BYTE  param[32];

    // for saving debugging status
    int save_stop;
    int save_cond;
} NDB;

// 函数声明
void ndb_init (NDB *ndb, CPU *cpu);
void ndb_free (NDB *ndb);
void ndb_reset(NDB *ndb);

// save & restore debugging status
void ndb_save   (NDB *ndb);
void ndb_restore(NDB *ndb);

// debug cpu
void ndb_cpu_debug(NDB *ndb);
void ndb_cpu_runto(NDB *ndb, int cond, void *param);
int  ndb_cpu_dasm (NDB *ndb, WORD pc, BYTE bytes[3], char *str);
void ndb_cpu_dump (NDB *ndb, char *str);

#endif






