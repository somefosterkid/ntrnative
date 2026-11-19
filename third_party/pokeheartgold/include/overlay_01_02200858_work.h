#ifndef POKEHEARTGOLD_OVERLAY_01_02200858_WORK_H
#define POKEHEARTGOLD_OVERLAY_01_02200858_WORK_H

#include "simple3dmodel.h"

struct FieldEffectManager;
struct PlayerAvatar;

typedef struct UnkStruct_021F61E8 {
    struct FieldEffectManager *unk_00;
    Simple3DModel unk_04;
    Simple3DRenderObj unk_18;
} UnkStruct_021F61E8;

typedef struct UnkStruct_021F6218 {
    struct FieldEffectManager *unk_00;
    UnkStruct_021F61E8 *unk_04;
    struct PlayerAvatar *playerAvatar;
} UnkStruct_021F6218;

// Work area of the player's overworld animation (OverworldAnimManagerFuncs ov01_02209340).
typedef struct UnkStruct_021F6264 {
    int unk_00;
    int unk_04;
    u8 unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    UnkStruct_021F6218 unk_24;
} UnkStruct_021F6264;

#endif // POKEHEARTGOLD_OVERLAY_01_02200858_WORK_H
