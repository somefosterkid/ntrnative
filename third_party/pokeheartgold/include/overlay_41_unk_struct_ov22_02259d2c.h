#ifndef POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_02259D2C_H
#define POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_02259D2C_H

#include "touchscreen.h"
#include "overlay_41_unk_struct_ov22_0225a154.h"

// Contest app touch-button block (DS size 0x84): four icon buttons, one text button, five hitboxes.
struct TouchHitboxController;
struct UnkStruct_02013534;

typedef struct UnkStruct_ov22_02259C58 {
    struct Sprite *unk_00;
    int unk_04;
    void *unk_08;
    void (*unk_0C)(struct UnkStruct_ov22_02259C58 *param0, void *param1);
} UnkStruct_ov22_02259C58;

typedef struct UnkStruct_ov22_02259D2C {
    UnkStruct_ov22_02259C58 unk_00[4];
    UnkStruct_ov22_0225A154 unk_40;
    BOOL unk_60;
    struct UnkStruct_02013534 *unk_64;
    struct TouchHitboxController *unk_68;
    TouchscreenHitbox unk_6C[5];
    u8 *unk_80;
} UnkStruct_ov22_02259D2C;

#endif // POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_02259D2C_H
