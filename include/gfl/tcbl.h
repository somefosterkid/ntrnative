// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/tcbl.h, with Black's func_02031288.
#ifndef BWPORT_GFL_TCBL_H
#define BWPORT_GFL_TCBL_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Tasks with data of their own (tcbl.c). Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

typedef void (*TCBExFunc)(TCBEx *task, void *data);

// A manager of count tasks, each holding dataSize bytes of data. Tasks with more data allocate it from dataHeapId
TCBExManager *GFL_TCBExMgrCreate(HeapID heapId, HeapID dataHeapId, u32 count, u32 dataSize);
void GFL_TCBExMgrUpdate(TCBExManager *manager);
void GFL_TCBExMgrFree(TCBExManager *manager);
void GFL_TCBExMgrFreeTasks(TCBExManager *manager);
void GFL_TCBExRequestEnd(TCBEx *task);
TCBEx *GFL_TCBExMgrAddTask(TCBExManager *manager, TCBExFunc func, u32 dataSize, u32 priority);
// Sets the task's function. Black 2 has no such function in tcbl.c
void func_02031288(TCBEx *task, TCBExFunc func);
void *GFL_TCBExGetData(TCBEx *task);

#endif // BWPORT_GFL_TCBL_H
