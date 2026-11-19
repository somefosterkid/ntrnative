#ifndef POKEHEARTGOLD_UNK_02095DF4_H
#define POKEHEARTGOLD_UNK_02095DF4_H

#include "script.h"

typedef struct UnkStruct_02095E30 {
    SaveData *saveData;
    u32 mode;
    u16 unk08;
    u16 unk0A;
    u8 unk0C;
    u8 unk0D;
    u8 unk0E;
    u8 unk0F;
} UnkStruct_02095E30;

typedef struct UnkStruct_02095DF4 {
    u32 unk00;
    UnkStruct_02095E30 *unk04;
    u32 unk08;
    u16 unk0C;
    u16 unk0E;
    u16 *unk10;
    u16 *unk14;
    u16 *unk18;
    u16 *unk1C;
} UnkStruct_02095DF4;

void sub_02095DF4(FieldSystem *fieldSystem, u8 a1, u8 a2, u8 a3, u16 *a4, u16 *a5, u16 *a6, u16 *a7);

#endif // POKEHEARTGOLD_UNK_02095DF4_H
