#ifndef POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV70_02260744_H
#define POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV70_02260744_H

#include "global.h"

#include "bg_window.h"

struct NNSG2dScreenData;
struct TouchHitboxController;

typedef struct UnkStruct_ov70_02260744 {
    s16 unk_00;
    u8 unk_02;
    u8 unk_03;
    u16 unk_04;
    u16 unk_06;
    u16 unk_08;
    u16 unk_0A;
    Window unk_0C;
    struct TouchHitboxController *unk_1C;
    void *unk_20[3];
    struct NNSG2dScreenData *unk_2C[3];
} UnkStruct_ov70_02260744;

// sizeof(UnkStruct_ov70_02260744) == 0x38

#endif // POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV70_02260744_H
