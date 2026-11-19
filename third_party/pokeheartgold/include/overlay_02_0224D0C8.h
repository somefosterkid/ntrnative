#ifndef POKEHEARTGOLD_OVERLAY_02_0224D0C8_H
#define POKEHEARTGOLD_OVERLAY_02_0224D0C8_H

#include "global.h"

#include "field/field_3d_object_task.h"
#include "field/overlay_01_021FB878.h"
#include "field_types_def.h"

// A 3D model with up to four animations, built by ov02_0224D0C8 for the ov02 Field3dObjectTask effects.
typedef struct Field3dAnimatedModel {
    Field3dObject object;
    Field3dModel model;
    Field3DModelAnimation anims[4];
    u32 animCount;
} Field3dAnimatedModel;

// Task data of those effects: the model, the allocator its animations load through and a done flag.
typedef struct Field3dEffectData {
    Field3dAnimatedModel model;
    NNSFndAllocator allocator;
    s32 state;
} Field3dEffectData;

// The same block for ov02_0224D5B4, which clears only a halfword flag.
typedef struct Field3dEffectData16 {
    Field3dAnimatedModel model;
    NNSFndAllocator allocator;
    u16 state;
    u16 padding;
} Field3dEffectData16;

// Task data of the camera shift effects (ov02_0224D9C0, ov02_0224DAA4): the model and its allocator, then the camera
// position and target it restores and the offset it shifts both by.
typedef struct Field3dCameraEffectData {
    Field3dAnimatedModel model;
    NNSFndAllocator allocator;
    VecFx32 cameraPos;
    VecFx32 cameraTarget;
    VecFx32 cameraOffset;
    s8 delay;
    u8 shifted;
    u8 shiftCount;
    u8 state;
} Field3dCameraEffectData;

#ifndef SDK_PORT
// DS layout checks: the pointer fields are 4 bytes there (the PC build's are wider)
typedef char Field3dAnimatedModel_size_check[(sizeof(Field3dAnimatedModel) == 0xDC) ? 1 : -1];
typedef char Field3dEffectData_size_check[(sizeof(Field3dEffectData) == 0xF0) ? 1 : -1];
typedef char Field3dEffectData16_size_check[(sizeof(Field3dEffectData16) == 0xF0) ? 1 : -1];
typedef char Field3dCameraEffectData_size_check[(sizeof(Field3dCameraEffectData) == 0x114) ? 1 : -1];
#endif

void ov02_0224D0C8(Field3dAnimatedModel *model, u32 modelFileId, u32 animFileId, u32 animCount, NNSFndAllocator *allocator);
void ov02_0224D144(Field3dAnimatedModel *model, NNSFndAllocator *allocator);
BOOL ov02_0224D178(Field3dAnimatedModel *model);
void ov02_0224D1DC(Field3dObject *object);
void ov02_0224D1AC(Field3dAnimatedModel *model);
void ov02_0224D5B4(Field3dObjectTask *task, FieldSystem *fieldSystem, void *taskData);

#endif // POKEHEARTGOLD_OVERLAY_02_0224D0C8_H
