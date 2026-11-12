// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/tcb.h, the lines in use so far.
#ifndef BWPORT_GFL_TCB_H
#define BWPORT_GFL_TCB_H

#include "types.h"
#include "struct_decls.h"

// Tasks that run every frame or every VBlank (tcb.c). Names from swan (https://github.com/ds-pokemon-hacking/swan,
// GPL-3.0)

typedef void (*TCBFunc)(TCB *tcb, void *data);
typedef void (*VBlankCallback)(void *data);

TCB *GFL_VBlankTCBAdd(TCBFunc func, void *data, u32 priority);
BOOL GFL_TCBRemove(TCB *tcb);
// Adds a task that runs after the tasks of lower or the same priority
TCB *GFL_TCBMgrAddTask(TCBManager *manager, TCBFunc func, void *data, u32 priority);
void *GFL_TCBGetData(TCB *tcb);
// A single callback that runs every VBlank, for when the tasks cannot
BOOL GFL_VBlankSetCallback(VBlankCallback callback, void *data);
void GFL_VBlankResetCallback(void);

// A manager of count tasks, in a buffer of GFL_TCBMgrCalcAllocSize(count) bytes
u32 GFL_TCBMgrCalcAllocSize(u32 count);
TCBManager *GFL_TCBMgrCreate(u32 count, void *buffer);
void GFL_TCBMgrUpdate(TCBManager *manager);
// pokebw2's func_0203a610
void func_02030da4(TCBManager *manager);

TCBManager *GFL_VBlankGetTCBMgr(void);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
TCB *GFL_HBlankTCBAdd(TCBFunc func, void *data, u32 priority);

#endif // BWPORT_GFL_TCB_H
