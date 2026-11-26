#ifndef POKEHEARTGOLD_OVERLAY_91_UNK_STRUCT_OV115_02260440_H
#define POKEHEARTGOLD_OVERLAY_91_UNK_STRUCT_OV115_02260440_H

// Launch args of the Wi-Fi Plaza minigames: the plaza record by value, then the save data and flags.
// Built by the plaza (overlay 45) and by the field (sub_02078C18 / sub_02078C74 / sub_02078CC8).

#include "overlay_91_unk_struct_ov66_02230dbc.h"

struct SaveData;

// overlays 91 and 93 (_02101260[0], _02101260[4])
typedef struct UnkStruct_ov115_02260440 {
    UnkStruct_ov66_02230DBC unk_00;
    struct SaveData *saveData;
    u8 unk_38;
    u8 unk_39;
    u8 padding[2];
} UnkStruct_ov115_02260440;

// sizeof(UnkStruct_ov115_02260440) == 0x3c

// overlay 92 (_02101260[1])
typedef struct UnkStruct_ov66_02231134 {
    UnkStruct_ov66_02230DBC unk_00;
    struct SaveData *saveData;
    BOOL unk_38;
    u32 unk_3C;
} UnkStruct_ov66_02231134;

// sizeof(UnkStruct_ov66_02231134) == 0x40

#endif // POKEHEARTGOLD_OVERLAY_91_UNK_STRUCT_OV115_02260440_H
