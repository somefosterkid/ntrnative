#ifndef POKEHEARTGOLD_OVERLAY_01_021FF6B0_H
#define POKEHEARTGOLD_OVERLAY_01_021FF6B0_H

#include "global.h"
#include "overlay_01_021F1348.h"
#include "field_types_def.h"
#include "overworld_anim_manager.h"

// The user data ov01_021FF74C builds on its stack and ov01_021F1620 hands to ov01_021FF7B0 (ov01_02200140 for the
// ov01_02209294 sibling): 0x10 bytes at 32 bits, four pointers in it.
typedef struct UnkStruct_ov5_021F3F10 {
    FieldSystem *fieldSystem;
    FieldEffectManager *unk_04;
    FieldEffectRendererContext *unk_08;
    LocalMapObject *unk_0C;
} UnkStruct_ov5_021F3F10;

// The context of the ov01_022091EC and ov01_02209294 renderer anim managers (0x24 bytes at 32 bits; word 0 of those
// tables is its size, so the host tables must use sizeof(UnkStruct_ov5_021F3FB8)).
typedef struct UnkStruct_ov5_021F3FB8 {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    UnkStruct_ov5_021F3F10 unk_10;
    Billboard *unk_20;
} UnkStruct_ov5_021F3FB8;

void *ov01_021FF6B0(FieldEffectManager *fieldEffMan);

void ov01_021FF6CC(void *arg0);
void ov01_021FF724(FieldEffectRendererContext *ctx);

int ov01_021FF7B0(OverworldAnimManager *manager, UnkStruct_ov5_021F3FB8 *work);
void ov01_021FF7F4(OverworldAnimManager *manager, UnkStruct_ov5_021F3FB8 *work);
void ov01_021FF800(OverworldAnimManager *manager, UnkStruct_ov5_021F3FB8 *work);
void ov01_021FF830(OverworldAnimManager *manager, UnkStruct_ov5_021F3FB8 *work);

void ov01_021FF74C(LocalMapObject *mapObj);

#endif // POKEHEARTGOLD_OVERLAY_01_021FF6B0_H
