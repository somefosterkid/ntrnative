#ifndef POKEHEARTGOLD_OVERLAY_45_UNK_STRUCT_OV66_02234798_H
#define POKEHEARTGOLD_OVERLAY_45_UNK_STRUCT_OV66_02234798_H

#include "global.h"

#include "billboard.h"
#include "field/overlay_01_021FB878.h"

struct UnkStruct_ov63_0222BEC0_t;

typedef struct UnkStruct_ov63_0222BEC0_t UnkStruct_ov63_0222BEC0;

// A Wi-Fi Plaza billboard actor: a Billboard, its Field3dObject and the animation state (sizeof == 0x90 on the DS).
typedef struct UnkStruct_ov66_02234798_t {
    u8 unk_00_0 : 4;
    u8 unk_00_4 : 2;
    u8 unk_00_6 : 2;
    u8 unk_01_0 : 1;
    u8 unk_01_1 : 7;
    u8 unk_02;
    u8 unk_03;
    const UnkStruct_ov63_0222BEC0 *unk_04;
    Billboard *unk_08;
    Field3dObject unk_0C;
    u8 unk_84;
    u8 unk_85;
    u16 unk_86;
    fx32 unk_88;
    int unk_8C;
} UnkStruct_ov66_02234798;

#endif // POKEHEARTGOLD_OVERLAY_45_UNK_STRUCT_OV66_02234798_H
