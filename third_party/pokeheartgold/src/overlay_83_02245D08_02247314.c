#include "global.h"

#include "filesystem.h"
#include "party_menu.h"
#include "unk_0200A090.h"

void ov83_02247314(void *arg0);

void ov83_02247314(void *arg0) {
    NARC *narc;

    narc = NARC_New(NARC_graphic_plist_gra, HEAP_ID_107);
    *(SpriteResource **)((u8 *)arg0 + 0x14C) = AddCharResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)arg0 + 0x12C), narc, (s32)sub_0207CA9C(), 0, 1, 1, HEAP_ID_107);
    *(SpriteResource **)((u8 *)arg0 + 0x150) = AddPlttResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)arg0 + 0x130), NARC_graphic_plist_gra, (s32)sub_0207CAA0(), 0, 1, 1, 1, HEAP_ID_107);
    *(SpriteResource **)((u8 *)arg0 + 0x154) = AddCellOrAnimResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)arg0 + 0x134), narc, (s32)sub_0207CAA4(), 0, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_107);
    *(SpriteResource **)((u8 *)arg0 + 0x158) = AddCellOrAnimResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)arg0 + 0x138), narc, (s32)sub_0207CAA8(), 0, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_107);
    NARC_Delete(narc);
}

void ov83_022473BC(void *arg0);

void ov83_022473BC(void *arg0) {
    NARC *narc;

    narc = NARC_New(NARC_a_0_0_8, HEAP_ID_107);
    *(SpriteResource **)((u8 *)arg0 + 0x15C) = AddCharResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)arg0 + 0x12C), narc, 0x4C, 0, 2, 2, HEAP_ID_107);
    *(SpriteResource **)((u8 *)arg0 + 0x160) = AddPlttResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)arg0 + 0x130), narc, 0x4B, 0, 2, 2, 1, HEAP_ID_107);
    *(SpriteResource **)((u8 *)arg0 + 0x164) = AddCellOrAnimResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)arg0 + 0x134), narc, 0x4D, 0, 2, GF_GFX_RES_TYPE_CELL, HEAP_ID_107);
    *(SpriteResource **)((u8 *)arg0 + 0x168) = AddCellOrAnimResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)arg0 + 0x138), narc, 0x4E, 0, 2, GF_GFX_RES_TYPE_ANIM, HEAP_ID_107);
    NARC_Delete(narc);
}
