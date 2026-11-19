#include "global.h"

#include "constants/heap.h"

#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_115.h"
#include "palette.h"
#include "sprite.h"


s32 ov01_021EFCDC(void *, void *);
s32 ov115_0225F978(void *, s32, s32 *);
extern s32 ov115_02260388;
extern s32 ov115_02260390;
extern s32 ov115_02260398;
extern s32 ov115_022603A0;
extern s32 ov115_022603A8;

void ov115_02260254(Sprite *param_1, enum HeapID param_2, s32 param_3, u8 param_4, u16 param_5) {
    u16 *dest;
    void *ptr;
    NNSG2dImagePaletteProxy *pImg;
    u32 offset;
    NNSG2dPaletteData *pNStack_18;

    dest = Heap_Alloc(param_2, 0x20);
    ptr = GfGfxLoader_GetPlttData(NARC_a_1_0_9, param_3, &pNStack_18, param_2);
    BlendPalette(pNStack_18->pRawData, dest, 0x10, param_4, param_5);
    pImg = Sprite_GetPaletteProxy(param_1);
    DC_FlushRange(dest, 0x20);
    offset = NNS_G2dGetImagePaletteLocation(pImg, NNS_G2D_VRAM_TYPE_2DMAIN);
    GX_LoadOBJPltt(dest, offset, 0x20);
    Heap_Free(dest);
    Heap_Free(ptr);
    return;
}

void ov115_022602C0(void *arg0, void *arg1) {
    if (ov115_0225F978(arg1, 4, &ov115_02260388) == 1) {
        ov01_021EFCDC(arg1, arg0);
    }
}

void ov115_022602E4(void *arg0, void *arg1) {
    if (ov115_0225F978(arg1, 4, &ov115_02260390) == 1) {
        ov01_021EFCDC(arg1, arg0);
    }
}

void ov115_02260308(void *arg0, void *arg1) {
    if (ov115_0225F978(arg1, 4, &ov115_02260398) == 1) {
        ov01_021EFCDC(arg1, arg0);
    }
}

void ov115_0226032C(void *arg0, void *arg1) {
    if (ov115_0225F978(arg1, 4, &ov115_022603A0) == 1) {
        ov01_021EFCDC(arg1, arg0);
    }
}

void ov115_02260350(void *arg0, void *arg1) {
    if (ov115_0225F978(arg1, 4, &ov115_022603A8) == 1) {
        ov01_021EFCDC(arg1, arg0);
    }
}
