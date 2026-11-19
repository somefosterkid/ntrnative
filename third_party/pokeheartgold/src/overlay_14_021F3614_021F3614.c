#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "unk_02013FDC.h"
#include "wild_encounter.h"
#include "overlay_14_box_funcs.h"

typedef enum GFPalSlotOffset GFPalSlotOffset;

extern UnkStruct_02014E30 ov14_021F8098;

void ov14_021F3614(UnkStruct_ov14_boxsys *param_1, UnkStruct_ov14_mon *param_2, s32 param_3) {
    void *dest;
    u32 species;
    NNSG2dImageProxy *pImg;
    u32 offset;
    NNSG2dImagePaletteProxy *pImg_00;
    GFPalSlotOffset palSlotOffset;
    PokepicTemplate PStack_24;
    UnkStruct_02014E30 UStack_34;

    UStack_34 = ov14_021F8098;
    dest = Heap_AllocAtEnd(HEAP_ID_10, 0xc80);
    GetBoxmonSpriteCharAndPlttNarcIds(&PStack_24, param_2->boxmon, 2, 0);
    if (param_2->isEgg == 1 && param_2->species == 0x147) {
        species = 0x1ee;
    } else {
        species = param_2->species;
    }
    sub_02014510((NarcId)PStack_24.narcID, (s32)PStack_24.charDataID, HEAP_ID_10, &UStack_34, dest, param_2->personality, 0, 2, species);
    pImg = Sprite_GetImageProxy(param_1->sprites[param_3]->sprite);
    offset = NNS_G2dGetImageLocation(pImg, NNS_G2D_VRAM_TYPE_2DSUB);
    DC_FlushRange(dest, 0xc80);
    GXS_LoadOBJ(dest, offset, 0xc80);
    pImg_00 = Sprite_GetPaletteProxy(param_1->sprites[param_3]->sprite);
    palSlotOffset = (GFPalSlotOffset)NNS_G2dGetImagePaletteLocation(pImg_00, NNS_G2D_VRAM_TYPE_2DSUB);
    GfGfxLoader_GXLoadPal((NarcId)PStack_24.narcID, (s32)PStack_24.palDataID, GF_PAL_LOCATION_SUB_OBJ, palSlotOffset, 0x20, HEAP_ID_10);
    Heap_Free(dest);
    return;
}
