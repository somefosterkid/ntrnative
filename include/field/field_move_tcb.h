// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_move_tcb.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_FIELD_MOVE_TCB_H
#define BWPORT_FIELD_FIELD_MOVE_TCB_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void *FieldSurfTCB_Create(FieldPlayer *player, u32 direction, u32 tileType, HeapID heapId);
void *FieldWaterfallTCB_Create(FieldPlayer *player, u32 direction, u32 param, HeapID heapId);
BOOL FieldSurfTCB_CheckEnd(void *tcb);
BOOL FieldWaterfallTCB_CheckEnd(void *tcb);
void FieldWaterfallTCB_Free(void *tcb);
void FieldSurfTCB_Free(void *tcb);

#endif // BWPORT_FIELD_FIELD_MOVE_TCB_H
