#ifndef POKEHEARTGOLD_OVERLAY_01_021FCE98_H
#define POKEHEARTGOLD_OVERLAY_01_021FCE98_H

#include "global.h"

#include "field_move_environment.h"
#include "field_types_def.h"
#include "sys_task.h"
#include "field/overlay_01_021FB878.h"

// What Task_UseSweetScentInField puts in FieldMoveEnvironment::moveData in place of the FieldMoveData (0xC bytes on the
// DS): the cut in task, the Honey/Sweet Scent work block and the party slot taken from the FieldMoveData.
typedef struct SweetScentTaskData {
    SysTask *cutInTask;
    void *effectWork;
    u32 partySlot;
} SweetScentTaskData;

// The Sweet Scent field effect: one FIELD1 block holding the allocator its
// animations load through, the model, its three animations and the object.
typedef struct SweetScentEffect {
    NNSFndAllocator allocator;
    Field3dModel model;
    Field3DModelAnimation anims[3];
    Field3dObject object;
} SweetScentEffect;

#ifndef SDK_PORT
// DS layout check: the pointer fields are 4 bytes there (the PC build's are wider)
typedef char SweetScentEffect_size_check[(sizeof(SweetScentEffect) == 0xD4) ? 1 : -1];
#endif

void ov01_021FCFEC(FieldSystem *fieldSystem);
void ov01_021FD064(enum HeapID heapID, FieldSystem *fieldSystem, SweetScentEffect *effect);
void ov01_021FD128(SweetScentEffect *effect);

u32 ov01_021FD154(Field3DModelAnimation *anims, u32 count);
void ov01_021FD190(Field3DModelAnimation *anims, u32 count, fx32 frame);

#endif // POKEHEARTGOLD_OVERLAY_01_021FCE98_H
