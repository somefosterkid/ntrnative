#ifndef POKEHEARTGOLD_OVERLAY_01_UNK_STRUCT_OV5_021F0FB8_H
#define POKEHEARTGOLD_OVERLAY_01_UNK_STRUCT_OV5_021F0FB8_H

// UnkStruct_ov5_021F0FB8: merged by tools/hg/promote.py from 3 m2c local copies (1 variants, base
// src/overlay_01_021FCD2C_021FCD2C.c) and 9 accesses; fields at every accessed offset, pads elsewhere.

struct Camera;
struct FieldSystem;

typedef struct UnkStruct_ov5_021F0FB8 {
    u32 heapID;
    int unk_04;
    int unk_08;
    int unk_0C;
    fx32 unk_10;
    fx32 unk_14;
    fx32 unk_18;
    u32 unk_1C;
    fx32 unk_20;
    u32 unk_24;
    fx32 unk_28;
    struct FieldSystem *fieldSystem;
    struct Camera *camera;
} UnkStruct_ov5_021F0FB8;

// sizeof(UnkStruct_ov5_021F0FB8) == 0x34

#endif // POKEHEARTGOLD_OVERLAY_01_UNK_STRUCT_OV5_021F0FB8_H
