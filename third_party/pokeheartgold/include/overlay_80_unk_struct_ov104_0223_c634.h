#ifndef POKEHEARTGOLD_OVERLAY_80_UNK_STRUCT_OV104_0223_C634_H
#define POKEHEARTGOLD_OVERLAY_80_UNK_STRUCT_OV104_0223_C634_H

// UnkStruct_ov104_0223C634: merged by tools/hg/promote.py from 14 m2c local copies (1 variants, base
// src/overlay_80_0222BDF4_0222BE9C_0222BE9C.c) and 2 accesses; fields at every accessed offset, pads elsewhere.

struct SysTask;
struct UnkStruct_ov63_0222BEC0_t;

typedef struct UnkStruct_ov104_0223C634 {
    struct UnkStruct_ov63_0222BEC0_t *unk_00;
    void *sprite;
    struct { u16 unk_00; u16 unk_02; u16 unk_04; s16 unk_06; s16 unk_08; u8 unk_0A; u8 unk_0B; u8 unk_0C; u8 unk_E_pad[0x1]; struct { u8 unk_00; u8 unk_01; s16 unk_02[7]; } unk_0E; } unk_08;
    struct { u8 unk_00; u8 unk_01; s16 unk_02[7]; } unk_26;
    u8 pad_36[0x2];
    struct SysTask *movementTask;
} UnkStruct_ov104_0223C634;

// sizeof(UnkStruct_ov104_0223C634) == 0x3c

struct BattleFrontier;
UnkStruct_ov104_0223C634 *sub_0209686C(struct BattleFrontier *frontier, s32 idx);

#endif // POKEHEARTGOLD_OVERLAY_80_UNK_STRUCT_OV104_0223_C634_H
