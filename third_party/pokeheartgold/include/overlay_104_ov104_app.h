#ifndef POKEHEARTGOLD_OVERLAY_104_OV104_APP_H
#define POKEHEARTGOLD_OVERLAY_104_OV104_APP_H

// ov104_App: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_104_021E5BEC_021E5CC8_021E5CC8.c) and 16 accesses; fields at every accessed offset, pads elsewhere.

struct NNSG3dAnmObj_;
struct NNSG3dJntAnmResult_;
struct NNSG3dMatAnmResult_;
struct NNSG3dResMdl_;

typedef struct ov104_App {
    u8 pad_00[4];
    struct { NNSG3dRenderObj renderObj; struct NNSG3dResMdl_ *model; void *resource; void *animFiles[2]; struct NNSG3dAnmObj_ *animObjs[2]; } slots[3];
    u8 pad_a[0];
    u8 allocator[16];
    u8 pad_b[12];
    u8 flagA;
    u8 flagB;
    u8 flagC;
    u8 pad_c;
    u8 pad_d[4];
    u8 *table;
} ov104_App;

// sizeof(ov104_App) == 0x170

#endif // POKEHEARTGOLD_OVERLAY_104_OV104_APP_H
