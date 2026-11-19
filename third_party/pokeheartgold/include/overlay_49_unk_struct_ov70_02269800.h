#ifndef POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV70_02269800_H
#define POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV70_02269800_H

// UnkStruct_ov70_02269800: merged by tools/hg/promote.py from 28 m2c local copies (1 variants, base
// src/overlay_49_0225A428_0225A490.c) and 10 accesses; fields at every accessed offset, pads elsewhere.

struct NNSG3dAnmObj_;
struct NNSG3dJntAnmResult_;
struct NNSG3dMatAnmResult_;
struct NNSG3dResMdl_;
struct UnkStruct_ov70_0225CC54_t;

typedef struct UnkStruct_ov70_02269800 {
    u8 unk_00;
    u8 unk_01;
    s16 unk_02;
    u32 unk_04;
    struct UnkStruct_ov70_0225CC54_t *unk_08;
    struct { NNSG3dRenderObj renderObj; struct { fx32 x; fx32 y; fx32 z; } position; struct { fx32 x; fx32 y; fx32 z; } scale; BOOL visible; u16 rotation[3]; u8 padding_76[2]; } unk_0C[18];
    void *unk_87C[18];
    s32 unk_8C4[2][18];
    u8 pad_954[0x11];
    u8 unk_965;
    u8 pad_966[0x3aa];
} UnkStruct_ov70_02269800;

// sizeof(UnkStruct_ov70_02269800) == 0xd10

#endif // POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV70_02269800_H
