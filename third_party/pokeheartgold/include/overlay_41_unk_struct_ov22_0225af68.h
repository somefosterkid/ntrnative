#ifndef POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225AF68_H
#define POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225AF68_H

#include "overlay_41_unk_struct_ov22_02257964.h"

struct BgConfig;

// UnkStruct_ov22_022597BC / UnkStruct_ov22_02259820 are the components of UnkStruct_ov22_0225AF68 (the 0x1c-byte
// state of ov41_0224B4E8 .. ov41_0224B50C).
typedef struct UnkStruct_ov22_022597BC {
    struct BgConfig *unk_00;
    u32 narcID;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
} UnkStruct_ov22_022597BC;

typedef struct UnkStruct_ov22_02259820 {
    const UnkStruct_ov22_022597BC *unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_ov22_02259820;

typedef struct UnkStruct_ov22_0225AF68 {
    const UnkStruct_ov22_02257964 *unk_00;
    UnkStruct_ov22_02259820 unk_04;
} UnkStruct_ov22_0225AF68;

// sizeof(UnkStruct_ov22_0225AF68) == 0x1c on DS

#endif // POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225AF68_H
