#ifndef POKEHEARTGOLD_OVERLAY_02_FIELD3D_EFFECTS_H
#define POKEHEARTGOLD_OVERLAY_02_FIELD3D_EFFECTS_H

#include "field/field_3d_object_task.h"
#include "field/overlay_01_021FB878.h"
#include "field_types_def.h"
#include "heap.h"

#define OV02_FIELD3D_EFFECT_ANIM_COUNT 4

// One playing 3D effect: a Field3dObject drawn with four animations, "playing" while they run.
typedef struct Ov02Field3dEffectSlot {
    Field3dObject object;
    Field3DModelAnimation anims[OV02_FIELD3D_EFFECT_ANIM_COUNT];
    u32 playing;
} Ov02Field3dEffectSlot; // 0xCC

// Field3dObjectTask data of ov02_0224D880 (16 slots spawned at the player every few frames, offset by the player's movement)
typedef struct Ov02App_0224D880 {
    Field3dModel model;
    Ov02Field3dEffectSlot slots[16];
    void *animFiles[OV02_FIELD3D_EFFECT_ANIM_COUNT];
    FieldSystem *fieldSystem;
    int spawnTimer;
    fx32 offsetY;
    fx32 offsetZ;
    VecFx32 lastPlayerPos;
    NNSFndAllocator allocator;
    u32 state;
} Ov02App_0224D880; // 0xD10

// Field3dObjectTask data of ov02_0224DCB0 (18 slots spawned at the player and, when enabled, the follower mon)
typedef struct Ov02App_0224DCB0 {
    Field3dModel model;
    Ov02Field3dEffectSlot slots[18];
    void *animFiles[OV02_FIELD3D_EFFECT_ANIM_COUNT];
    FieldSystem *fieldSystem;
    int spawnTimer;
    fx32 offsetY;
    fx32 offsetZ;
    NNSFndAllocator allocator;
    u16 suspended;
    u16 followMonEnabled;
} Ov02App_0224DCB0; // 0xE9C

void ov02_0224D698(Ov02Field3dEffectSlot *slot, PlayerAvatar *avatar, fx32 offsetY, fx32 offsetZ);
void ov02_0224D700(Ov02Field3dEffectSlot *slot);
void ov02_0224D73C(Ov02Field3dEffectSlot *slot, Field3dModel *model, NNSFndAllocator *allocator, void **animFiles);
void ov02_0224D788(Ov02Field3dEffectSlot *slot, NNSFndAllocator *allocator);
// One slot of the ov02_0224D43C Field3dObjectTask data: Ov02Field3dEffectSlot with its own model.
typedef struct UnkStruct_ov6_02248140 {
    Field3dObject unk_00;
    Field3dModel unk_78;
    Field3DModelAnimation unk_88[OV02_FIELD3D_EFFECT_ANIM_COUNT];
    u32 unk_D8;
} UnkStruct_ov6_02248140; // 0xDC

// Field3dObjectTask data of ov02_0224D43C (two slots)
typedef struct UnkStruct_ov6_02248498 {
    UnkStruct_ov6_02248140 unk_00[2];
    NNSFndAllocator unk_1B8;
    u16 unk_1C8;
    u16 unk_1CA;
} UnkStruct_ov6_02248498; // 0x1CC

void ov02_0224D7B0(Ov02App_0224D880 *env);
void ov02_0224D820(Ov02App_0224D880 *env);
void ov02_0224D868(Ov02App_0224D880 *env);
void ov02_0224D880(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

void ov02_0224DCB0(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224DD38(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224DD4C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224DD8C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224DDC8(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224DE10(Ov02Field3dEffectSlot *slot, const VecFx32 *pos, fx32 offsetY, fx32 offsetZ);
void ov02_0224DE6C(Ov02Field3dEffectSlot *slot);
void ov02_0224DEA8(Ov02Field3dEffectSlot *slot, Field3dModel *model, NNSFndAllocator *allocator, void **animFiles);
void ov02_0224DEF4(Ov02Field3dEffectSlot *slot, NNSFndAllocator *allocator);
void ov02_0224DF1C(Ov02App_0224DCB0 *env);
void ov02_0224E008(Ov02App_0224DCB0 *env);

#endif // POKEHEARTGOLD_OVERLAY_02_FIELD3D_EFFECTS_H
