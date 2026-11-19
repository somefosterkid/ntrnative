#ifndef POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV70_022615E8_H
#define POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV70_022615E8_H

// UnkStruct_ov70_022615E8: merged by tools/hg/promote.py from 10 m2c local copies (1 variants, base
// src/overlay_49_0225CC4C.c) and 3 accesses; fields at every accessed offset, pads elsewhere.

struct NNSG3dAnmObj_;
struct NNSG3dJntAnmResult_;
struct NNSG3dMatAnmResult_;
struct NNSG3dResMdl_;

typedef struct UnkStruct_ov70_022615E8 {
    BOOL unk_00;
    struct { NNSG3dRenderObj renderObj; struct { fx32 x; fx32 y; fx32 z; } position; struct { fx32 x; fx32 y; fx32 z; } scale; BOOL visible; u16 rotation[3]; u8 padding_76[2]; } unk_04[2];
    u32 unk_F4[5];
    s32 unk_108[5];
    u8 pad_11C[0xa];
    u8 unk_126;
    u8 unk_127;
    u32 unk_128;
} UnkStruct_ov70_022615E8;

// sizeof(UnkStruct_ov70_022615E8) == 0x12c

#endif // POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV70_022615E8_H
