#ifndef POKEHEARTGOLD_OVERLAY_43_UNK_STRUCT_OV64_0222_E060_H
#define POKEHEARTGOLD_OVERLAY_43_UNK_STRUCT_OV64_0222_E060_H

// UnkStruct_ov64_0222E060: merged by tools/hg/promote.py from 20 m2c local copies (5 variants, base
// src/overlay_43_0222B0A0_0222B5D0_0222B5D0.c) and 56 accesses; fields at every accessed offset, pads elsewhere.

struct SaveData;
struct String;

typedef struct UnkStruct_ov64_0222E060 {
    u8 unk_0[4];
    struct SaveData *saveData;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    s8 unk_07;
    u8 pad_C_[0x4];
    u32 unk_10;
    struct { u32 unk_00; u8 unk_04[32]; } unk_08;
    u8 pad_38[32];
    struct { struct String *unk_00; struct String *unk_04; } unk_2C;
    struct { u16 *unk_00; BOOL unk_04; } unk_34;
} UnkStruct_ov64_0222E060;

// sizeof(UnkStruct_ov64_0222E060) == 0x68

#endif // POKEHEARTGOLD_OVERLAY_43_UNK_STRUCT_OV64_0222_E060_H
