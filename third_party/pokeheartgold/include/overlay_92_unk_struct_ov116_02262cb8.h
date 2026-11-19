#ifndef POKEHEARTGOLD_OVERLAY_92_UNK_STRUCT_OV116_02262CB8_H
#define POKEHEARTGOLD_OVERLAY_92_UNK_STRUCT_OV116_02262CB8_H

#include "overlay_92_unk_struct_ov116_022649e4.h"

struct ManagedSprite;

// Shared by ov92_0225EDB4 (allocates UnkStruct_ov116_02262CB8) and ov92_0225F254 (its SysTask).

// Quick wipe task of the ov116 player sprites: slides a sprite up, then down (heap block 0x30 on DS).
// unk_2C is a pointer to the 0x38 graphics context (UnkStruct_ov116_022649E4).
typedef struct UnkStruct_ov116_02262CB8 {
    int unk_00;
    int unk_04;
    int unk_08;
    struct ManagedSprite *unk_0C;
    struct ManagedSprite *unk_10;
    UnkStruct_ov116_0226469C unk_14;
    UnkStruct_ov116_022649E4 *unk_2C;
} UnkStruct_ov116_02262CB8; // 0x30

#endif // POKEHEARTGOLD_OVERLAY_92_UNK_STRUCT_OV116_02262CB8_H
