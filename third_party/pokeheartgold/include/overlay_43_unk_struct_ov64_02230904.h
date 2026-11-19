#ifndef POKEHEARTGOLD_OVERLAY_43_UNK_STRUCT_OV64_02230904_H
#define POKEHEARTGOLD_OVERLAY_43_UNK_STRUCT_OV64_02230904_H

// UnkStruct_ov64_02230904: merged by tools/hg/promote.py from 11 m2c local copies (1 variants, base
// src/overlay_43_0222A154_0222A154.c) and 7 accesses; fields at every accessed offset, pads elsewhere.

struct NNSG2dScreenData;
struct Window;

typedef struct UnkStruct_ov64_02230904 {
    s16 unk_00;
    s16 unk_02;
    s32 unk_04;
    int unk_08;
    u16 unk_0C;
    u16 unk_0E;
    BOOL unk_10;
    struct { struct { s16 unk_00; s16 unk_02; u8 unk_04; u8 unk_05; } unk_00; u8 unk_8_pad[0x2]; u32 unk_08; struct Window *unk_0C[7]; u32 unk_28[7]; u8 unk_44[8]; } unk_14[3];
    struct { void *unk_00[7]; struct NNSG2dScreenData *unk_1C[7]; } unk_DC;
} UnkStruct_ov64_02230904;

// sizeof(UnkStruct_ov64_02230904) == 0x130

#endif // POKEHEARTGOLD_OVERLAY_43_UNK_STRUCT_OV64_02230904_H
