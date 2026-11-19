#ifndef POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225DAC0_H
#define POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225DAC0_H

// UnkStruct_ov114_0225DAC0: one of the 4 player panels at +0x2C0 (Platinum offsets); one of the sub-objects of UnkStruct_ov114_0225E854_t (the ov90 Wi-Fi Plaza minigame app, 0x664)

#include "global.h"
#include "bg_window.h"
#include "overlay_90_unk_struct_ov114_0225cbf0.h"

struct Sprite;

typedef struct UnkStruct_ov114_0225DAC0 {
    Window unk_00;
    struct Sprite *unk_10;
    struct Sprite *unk_14;
    u16 unk_18;
    u8 unk_1A;
    u8 unk_1B;
    UnkStruct_ov114_0225CBF0 unk_1C;
    s16 unk_30;
    s16 unk_32;
    s32 unk_34;
} UnkStruct_ov114_0225DAC0;

// sizeof(UnkStruct_ov114_0225DAC0) == 0x38

#endif // POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225DAC0_H
