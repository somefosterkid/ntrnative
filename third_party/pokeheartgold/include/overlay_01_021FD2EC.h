#ifndef POKEHEARTGOLD_OVERLAY_01_021FD2EC_H
#define POKEHEARTGOLD_OVERLAY_01_021FD2EC_H

#include "global.h"
#include "field_types_def.h"
#include "overworld_anim_manager.h"
#include "simple3dmodel.h"

struct FieldEffectManager;

// User data of the funcs table ov01_02208E1C (ov01_021FD2EC): the model follows `unk_08`.
typedef struct UnkStruct_021F121C {
    u32 unk_00; // sprite id
    struct FieldEffectManager *unk_04;
    LocalMapObject *unk_08;
} UnkStruct_021F121C;

// Context of the funcs table ov01_02208E1C (0x28 bytes on the DS): its init copies the user data to `unk_1C`.
typedef struct UnkStruct_021F1258 {
    int unk_00;
    u32 unk_04; // object id
    u32 unk_08; // map id
    VecFx32 unk_0C;
    Simple3DRenderObj *unk_18;
    UnkStruct_021F121C unk_1C;
} UnkStruct_021F1258;

Simple3DRenderObj *ov01_021FD2CC(struct FieldEffectManager *fieldEffMan, s32 arg1);
OverworldAnimManager *ov01_021FD2EC(LocalMapObject *param0, const VecFx32 *param1);
int ov01_021FD328(OverworldAnimManager *param0, UnkStruct_021F1258 *v0);
void ov01_021FD37C(OverworldAnimManager *param_1, UnkStruct_021F1258 *param_2);
void ov01_021FD3E0(OverworldAnimManager *param0, UnkStruct_021F1258 *v0);

#endif // POKEHEARTGOLD_OVERLAY_01_021FD2EC_H
