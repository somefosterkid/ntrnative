#ifndef POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225E0F8_H
#define POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225E0F8_H

// UnkStruct_ov114_0225E0F8: the four player profiles at +0x34; one of the sub-objects of UnkStruct_ov114_0225E854_t (the ov90 Wi-Fi Plaza minigame app, 0x664)

#include "global.h"

struct PlayerProfile;

typedef struct UnkStruct_ov114_0225E0F8 {
    u8 unk_00[4];
    u8 unk_04[4];
    struct PlayerProfile *unk_08[4];
} UnkStruct_ov114_0225E0F8;

// sizeof(UnkStruct_ov114_0225E0F8) == 0x18

#endif // POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225E0F8_H
