#ifndef POKEHEARTGOLD_OVERLAY_01_UNK_STRUCT_OV5_02202120_H
#define POKEHEARTGOLD_OVERLAY_01_UNK_STRUCT_OV5_02202120_H

#include "global.h"

struct FieldSystem;
struct ParticleSystem;

// An encounter effect's particle work (ov01_021F0DDC allocates it, ov01_02209B64 points to it, sizeof == 0x18 on the DS).
// Pointers are 8 bytes on the host: always the field, never an offset.
typedef struct UnkStruct_ov5_02202120 {
    u16 unk_00; // task state
    u8 unk_02;
    u8 unk_03; // set by ov01_021F0E74: the main task may go on
    struct FieldSystem *fieldSystem;
    struct ParticleSystem *unk_08;
    void *unk_0C; // the particle heap (0x4800 bytes)
    u32 unk_10;   // texture VRAM bytes handed out so far (ov01_021F1224)
    u32 unk_14;   // palette VRAM bytes handed out so far (ov01_021F1250)
} UnkStruct_ov5_02202120;

extern UnkStruct_ov5_02202120 *ov01_02209B64;

#endif // POKEHEARTGOLD_OVERLAY_01_UNK_STRUCT_OV5_02202120_H
