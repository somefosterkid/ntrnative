#ifndef POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225B1BC_H
#define POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225B1BC_H

#include "constants/heap.h"
#include "pokemon_types_def.h"
#include "overlay_41_unk_struct_ov22_0225a0e4_head.h"
#include "overlay_41_unk_struct_ov22_02259484.h"
#include "overlay_41_unk_struct_ov22_02254de0.h"
#include "overlay_41_unk_struct_ov22_02257964.h"
#include "overlay_41_unk_struct_ov22_0225af68.h"

// UnkStruct_ov22_0225B1BC: the 0x268-byte photo portrait object built by ov41_0224B630 and freed by ov41_0224B57C.
// Its start (unk_00) is the environment shared with UnkStruct_ov22_0225A0E4, the 0x6f0-byte object of
// OverlayManager ov41; fields at unk_184 and later are used by the portrait only. Field names are the DS offsets.
typedef struct UnkStruct_ov22_0225B1BC_t {
    UnkStruct_ov22_0225A0E4_Head unk_00;
    UnkStruct_ov22_02259484 unk_184;
    UnkStruct_ov22_02254DE0 *unk_18C;
    UnkStruct_ov22_02257964 unk_190;
    UnkStruct_ov22_0225AF68 unk_218;
    Pokemon *unk_234;
    enum HeapID heapID;
    int unk_23C;
    int unk_240;
    int unk_244;
    int unk_248;
    u16 unk_24C;
    VecFx32 unk_250;
    GXRgb unk_25C;
    BOOL unk_260;
    BOOL unk_264;
} UnkStruct_ov22_0225B1BC;

// sizeof(UnkStruct_ov22_0225B1BC) == 0x268 on DS

#endif // POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225B1BC_H
