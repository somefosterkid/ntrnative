#ifndef POKEHEARTGOLD_OVERLAY_01_021FD41C_H
#define POKEHEARTGOLD_OVERLAY_01_021FD41C_H

#include "global.h"
#include "field_types_def.h"
#include "overlay_01_021F1348.h"
#include "overworld_anim_manager.h"
#include "simple3dmodel.h"
#include "sys_task.h"

// FieldEffectRenderer 0's context (ov01_021FD41C): four models and their render objects, blended by the time of day
// (alpha `unk_10`, scale `unk_14`) by the task `unk_24`. 0x1C8 bytes at 32 bits.
#define OV01_021FD41C_MODEL_COUNT 4

typedef struct UnkStruct_ov5_021F1388 {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    fx32 unk_10;
    VecFx32 unk_14;
    FieldEffectManager *unk_20;
    SysTask *unk_24;
    Simple3DModel unk_28[OV01_021FD41C_MODEL_COUNT];
    Simple3DRenderObj unk_78[OV01_021FD41C_MODEL_COUNT];
} UnkStruct_ov5_021F1388;

// The userData the anim managers of renderer 0 are made with (copied into UnkStruct_ov5_021F15B4.unk_14).
typedef struct UnkStruct_ov5_021F1570 {
    FieldEffectManager *unk_00;
    UnkStruct_ov5_021F1388 *unk_04;
    LocalMapObject *unk_08;
} UnkStruct_ov5_021F1570;

// The anim managers' own context (0x20 bytes at 32 bits).
typedef struct UnkStruct_ov5_021F15B4 {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    UnkStruct_ov5_021F1570 unk_14;
} UnkStruct_ov5_021F15B4;

void *ov01_021FD41C(FieldEffectManager *fieldEffMan);
void ov01_021FD440(void *ctx);
void ov01_021FD458(UnkStruct_ov5_021F1388 *ctx);
void ov01_021FD47C(UnkStruct_ov5_021F1388 *ctx);
void ov01_021FD488(UnkStruct_ov5_021F1388 *ctx, VecFx32 *dst);
void ov01_021FD4F4(SysTask *param0, void *param1);
void ov01_021FD5CC(UnkStruct_ov5_021F1388 *ctx);
void ov01_021FD60C(UnkStruct_ov5_021F1388 *ctx);
void ov01_021FD624(UnkStruct_ov5_021F1388 *ctx, int alpha);

void ov01_021FD640(LocalMapObject *mapObject);
void ov01_021FD684(LocalMapObject *mapObject);
OverworldAnimManager *ov01_021FD8E8(LocalMapObject *mapObject, int userInt);

void ov01_021FD714(void);

void ov01_021FD9CC(u32 arg0, VecFx32 *pos);

#endif // POKEHEARTGOLD_OVERLAY_01_021FD41C_H
