#include "global.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "error_handling.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "palette.h"
#include "sprite.h"

s32 ov96_021EED14(Sprite *, void *, s32, s32);

void ov96_021EED64(void *arg0, void *arg1, s32 arg2);

void ov96_021EED70(Sprite **arg0, enum NarcId arg1, s32 arg2, s32 arg3, void *arg4, enum HeapID arg5);
extern s32 ov96_0221B1D4;
extern s32 ov96_0221B1D8;
s32 ov96_021EEDCC(s32 arg0);

void ov96_021EED64(void *arg0, void *arg1, s32 arg2) {
    ov96_021EED14(arg0, arg1, arg2, 0);
}

void ov96_021EED70(Sprite **arg0, enum NarcId narcId, s32 arg2, s32 arg3, void *arg4, enum HeapID heapID) {
    s32 memberNo;
    enum GFPalLoadLocation location;
    enum GFPalSlotOffset palSlotOffset;
    enum NNS_G2D_VRAM_TYPE type;
#if !(defined(SDK_PORT) || defined(NONMATCHING))
    s32 v0;
#endif

    memberNo = arg2;
    if (arg3 != 0) {
        type = NNS_G2D_VRAM_TYPE_2DMAIN;
        location = GF_PAL_LOCATION_MAIN_OBJ;
    } else {
        type = NNS_G2D_VRAM_TYPE_2DSUB;
        location = GF_PAL_LOCATION_SUB_OBJ;
    }
    palSlotOffset = (enum GFPalSlotOffset)NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(*arg0), type);
    GfGfxLoader_GXLoadPal(narcId, memberNo, location, palSlotOffset, 0x20U, heapID);
    if (arg4 != 0) {
#if defined(SDK_PORT) || defined(NONMATCHING)
        u8 *base = arg3 != 0 ? (u8 *)HW_OBJ_PLTT : (u8 *)HW_DB_OBJ_PLTT;
        TintPalette_GrayScale((u16 *)(base + palSlotOffset), 0x20);
#else
        if (arg3 != 0) {
            v0 = HW_OBJ_PLTT;
        } else {
            v0 = HW_DB_OBJ_PLTT;
        }
        TintPalette_GrayScale((u16 *)(palSlotOffset + v0), 0x20);
#endif
    }
}

s32 ov96_021EEDCC(s32 arg0) {
    s32 v0;
    s32 *v1;

    v1 = &ov96_0221B1D4;
    v0 = 0;
loop_1:
    if (v0 == 0xFFFF) {
        return 0;
    }
    if (arg0 == *v1) {
        return *(s32 *)(&ov96_0221B1D8 + v0 * 2);
    }
    v0 += 1;
    v1 += 2;
    if (v0 >= 0x270F) {
        GF_AssertFail();
        return 0;
    }
    goto loop_1;
}
