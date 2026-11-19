#ifndef POKEHEARTGOLD_OVERLAY_01_021FEA0C_H
#define POKEHEARTGOLD_OVERLAY_01_021FEA0C_H

#include "global.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_simple_renderers.h"
#include "overworld_anim_manager.h"

// User data of the funcs table ov01_022090FC (ov01_021FEAB0 builds it on the stack); init copies it to `unk_10` of the context.
typedef struct UnkStruct_ov5_021F28F4 {
    int unk_00; // direction
    FieldEffectManager *unk_04;
    FieldSystem *fieldSystem;
    UnkStruct_ov5_021F2874 *unk_0C; // renderer 5 (ov01_021FEA0C)
    LocalMapObject *unk_10;
} UnkStruct_ov5_021F28F4;

// Context of the funcs table ov01_022090FC (0x24 bytes at 32 bits).
typedef struct UnkStruct_ov5_021F2974 {
    int unk_00;
    int unk_04; // direction
    int unk_08; // tick counter
    int unk_0C; // synced to the map object (userInt)
    UnkStruct_ov5_021F28F4 unk_10;
} UnkStruct_ov5_021F2974;

void *ov01_021FEA0C(FieldEffectManager *fieldEffMan);

void ov01_021FEA20(void *arg0);

BOOL ov01_021FEB3C(OverworldAnimManager *animMan, UnkStruct_ov5_021F2974 *ctx);
void ov01_021FEB78(OverworldAnimManager *animMan, UnkStruct_ov5_021F2974 *ctx);
void ov01_021FEB8C(OverworldAnimManager *animMan, UnkStruct_ov5_021F2974 *ctx);
void ov01_021FEBC0(OverworldAnimManager *animMan, UnkStruct_ov5_021F2974 *ctx);

#endif // POKEHEARTGOLD_OVERLAY_01_021FEA0C_H
