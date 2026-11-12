#ifndef POKEBW2_FIELD_FIELD_ASYNC_PROC_H
#define POKEBW2_FIELD_FIELD_ASYNC_PROC_H

// Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// A process that the field runs beside itself, updated and drawn by the field's task managers

typedef void (*FieldAsyncProcCallback)(FieldAsyncProc *proc, Field *field, void *work);

typedef struct {
    // The priority of its tasks
    u32 priority;
    // The size of the work that is allocated for it, if not 0
    u16 workSize;
    FieldAsyncProcCallback init;
    FieldAsyncProcCallback free;
    FieldAsyncProcCallback update;
    FieldAsyncProcCallback draw;
} FieldAsyncProcDef;

FieldAsyncProcManager *Field_GetAsyncProcMgr(Field *field);
// Loads the overlay, unless it is OVERLAY_NONE, and starts the process. Returns NULL if there is no room for it
FieldAsyncProc *FieldAsyncProcManager_AddProc(u32 overlayId, FieldAsyncProcManager *mgr, const FieldAsyncProcDef *def);
void *FieldAsyncProc_GetData(FieldAsyncProc *proc);
void FieldAsyncProc_End(FieldAsyncProc *proc);
FieldAsyncProcManager *FieldAsyncProcManager_Create(Field *field, HeapID heapId, u32 count);
void FieldAsyncProcManager_Free(FieldAsyncProcManager *manager);
void FieldAsyncProcManager_Update(FieldAsyncProcManager *manager);
void FieldAsyncProcManager_Draw(FieldAsyncProcManager *manager);

#endif // POKEBW2_FIELD_FIELD_ASYNC_PROC_H
