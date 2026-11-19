#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "overlay_98.h"
#include "palette.h"
#include "sprite.h"


void ov98_0221EA4C(Sprite **arg0, enum NarcId narcId, s32 arg2, s32 arg3, s32 arg4, enum HeapID heapID) {
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

void ov98_0221EAA8(void *arg0, s32 memberNo, NNSG2dCharacterData **ppCharData) {
    GfGfxLoader_GetCharDataFromOpenNarc(*(NARC **)((u8 *)arg0 + 0x10), memberNo, 0, ppCharData, *(enum HeapID *)arg0);
}
