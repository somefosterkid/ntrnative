#ifndef POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225D338_H
#define POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225D338_H

// UnkStruct_ov114_0225D338: two text windows + their strings at +0x68; one of the sub-objects of UnkStruct_ov114_0225E854_t (the ov90 Wi-Fi Plaza minigame app, 0x664)

#include "global.h"
#include "bg_window.h"

struct String;

typedef struct UnkStruct_ov114_0225D338 {
    Window unk_00[2];
    u16 unk_20[2];
    struct String *unk_24[2];
    void *unk_2C[2];
    BOOL unk_34;
} UnkStruct_ov114_0225D338;

// sizeof(UnkStruct_ov114_0225D338) == 0x38

#endif // POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225D338_H
