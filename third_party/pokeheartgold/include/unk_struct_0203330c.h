#ifndef POKEHEARTGOLD_UNK_STRUCT_0203330C_H
#define POKEHEARTGOLD_UNK_STRUCT_0203330C_H

#include "global.h"

#include "pokemon_types_def.h"

// Union-room beacon payload: the userGameInfo of a scanned parent's WMBssDesc.
typedef struct UnkStruct_0203330C {
    u32 trainerId;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
    MailMessage sentence;
    u8 unk_10[32];
    u8 unk_30[32];
    u32 unk_50;
    u8 unk_54;
    u8 padding_55[3];
} UnkStruct_0203330C;

#endif // POKEHEARTGOLD_UNK_STRUCT_0203330C_H
