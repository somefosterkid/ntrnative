#ifndef POKEHEARTGOLD_OVERLAY_91_UNK_STRUCT_OV66_02230DBC_H
#define POKEHEARTGOLD_OVERLAY_91_UNK_STRUCT_OV66_02230DBC_H

// UnkStruct_ov66_02230DBC: the Wi-Fi Plaza minigame launch record (holds the plaza system at +0x30); one definition
// for overlays 8/45/49/90-93 (overlay_93_unk_struct_ov117_02261280_t.h used to carry a second copy).

#include "player_data.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"

typedef struct UnkStruct_ov66_02230DBC {
    u32 unk_00;
    UnkStruct_ov66_0222E294 unk_04;
    UnkStruct_ov66_02232068 unk_08;
    BOOL unk_1C;
    PlayerProfile *unk_20[4];
    WiFiPlazaSystem *unk_30;
    u32 unk_34;
    u8 unk_38;
    u8 unk_39;
    u8 pad_3A[2];
} UnkStruct_ov66_02230DBC;

// sizeof(UnkStruct_ov66_02230DBC) == 0x3c

#endif // POKEHEARTGOLD_OVERLAY_91_UNK_STRUCT_OV66_02230DBC_H
