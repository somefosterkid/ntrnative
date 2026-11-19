#ifndef POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225C76C_H
#define POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225C76C_H

// UnkStruct_ov114_0225C76C: the minigame launch arguments copied into the app at +0xC; one of the sub-objects of UnkStruct_ov114_0225E854_t (the ov90 Wi-Fi Plaza minigame app, 0x664)

#include "global.h"

struct SaveData;
struct UnkStruct_ov66_02230DBC;

typedef struct UnkStruct_ov114_0225C76C {
    struct SaveData *saveData;
    u8 unk_04[4];
    u8 unk_08;
    u8 unk_09;
    u8 unk_0A;
    u8 unk_0B;
    struct UnkStruct_ov66_02230DBC *unk_0C;
} UnkStruct_ov114_0225C76C;

// sizeof(UnkStruct_ov114_0225C76C) == 0x10

#endif // POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225C76C_H
