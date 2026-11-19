#include "global.h"

#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "item.h"
#include "party.h"
#include "pokemon_icon_idx.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"
#include "overlay_83_app_02246e08.h"

s32 ov83_022472DC(void);

s32 ov83_02247314(void *);

s32 ov83_022473BC(void *);

extern u8 ov83_02248178;

void ov83_02246E08(void *arg0, Party *arg1, s32 arg2);

void ov83_02246E08(void *arg0, Party *arg1, s32 arg2) {
    s32 sp18;
    Party *sp14;
    NARC *temp_r7;
    NARC *temp_r7_2;
    Pokemon *var_r0;
    GfGfxResType var_r7;
    void *var_r6_2;
    s32 var_r4_2;
    s32 var_r4_3;
    s32 var_r4_5;
    s32 var_r6_4;
    u8 *var_r4;
    void *var_r4_4;
    void *var_r5;
    void *var_r6;
    void *var_r6_3;

    var_r5 = arg0;
    sp14 = arg1;
    sp18 = arg2;
    GF_CreateVramTransferManager(0x20U, HEAP_ID_107);
    ov83_022472DC();
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x80, 0, 0x20, 0, 0x80, 0, 0x20, HEAP_ID_107);
    *(SpriteList **)var_r5 = G2dRenderer_Init(0x28, var_r5 + 4, HEAP_ID_107);
    var_r4 = &ov83_02248178;
    var_r7 = GF_GFX_RES_TYPE_CHAR;
    var_r6 = var_r5;
    do {
        *(GF_2DGfxResMan **)((u8 *)var_r6 + 0x12C) = Create2DGfxResObjMan((s32)*var_r4, var_r7, HEAP_ID_107);
        var_r4 += 1;
        var_r6 += 4;
        var_r7 += 1;
    } while (var_r7 < 4);
    ov83_022473BC(var_r5);
    *(SpriteResource **)((u8 *)var_r5 + 0x13C) = AddCharResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x12C), NARC_a_1_8_4, 0xF, 1, 0, 1, HEAP_ID_107);
    *(SpriteResource **)((u8 *)var_r5 + 0x140) = AddPlttResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x130), NARC_a_1_8_4, 0x37, 0, 0, 1, 4, HEAP_ID_107);
    *(SpriteResource **)((u8 *)var_r5 + 0x144) = AddCellOrAnimResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x134), NARC_a_1_8_4, 0x11, 1, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_107);
    *(SpriteResource **)((u8 *)var_r5 + 0x148) = AddCellOrAnimResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x138), NARC_a_1_8_4, 0x10, 1, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_107);
    temp_r7 = NARC_New(NARC_itemtool_itemdata_item_icon, HEAP_ID_107);
    var_r4_2 = 4;
    var_r6_2 = var_r5 + 0x40;
    do {
        *(SpriteResource **)((u8 *)var_r6_2 + 0x13C) = AddCharResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x12C), temp_r7, GetItemIndexMapping(0U, 1), 0, var_r4_2, 1, HEAP_ID_107);
        *(SpriteResource **)((u8 *)var_r6_2 + 0x140) = AddPlttResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x130), NARC_itemtool_itemdata_item_icon, GetItemIndexMapping(0U, 2), 0, var_r4_2, 1, 1, HEAP_ID_107);
        var_r4_2 += 1;
        var_r6_2 += 0x10;
    } while (var_r4_2 <= 9);
    *(SpriteResource **)((u8 *)var_r5 + 0x184) = AddCellOrAnimResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x134), temp_r7, GetItemIconCell(), 0, 4, GF_GFX_RES_TYPE_CELL, HEAP_ID_107);
    *(SpriteResource **)((u8 *)var_r5 + 0x188) = AddCellOrAnimResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x138), temp_r7, GetItemIconAnim(), 0, 4, GF_GFX_RES_TYPE_ANIM, HEAP_ID_107);
    *(SpriteResource **)((u8 *)var_r5 + 0x170) = AddPlttResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x130), NARC_itemtool_itemdata_item_icon, GetItemIndexMapping(0U, 2), 0, 3, 2, 1, HEAP_ID_107);
    NARC_Delete(temp_r7);
    *(SpriteResource **)((u8 *)var_r5 + 0x16C) = AddCharResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x12C), NARC_a_1_8_4, 0x24, 1, 3, 2, HEAP_ID_107);
    *(SpriteResource **)((u8 *)var_r5 + 0x174) = AddCellOrAnimResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x134), NARC_a_1_8_4, 0x26, 1, 3, GF_GFX_RES_TYPE_CELL, HEAP_ID_107);
    *(SpriteResource **)((u8 *)var_r5 + 0x178) = AddCellOrAnimResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x138), NARC_a_1_8_4, 0x25, 1, 3, GF_GFX_RES_TYPE_ANIM, HEAP_ID_107);
    ov83_02247314(var_r5);
    temp_r7_2 = NARC_New(NARC_poketool_icongra_poke_icon, HEAP_ID_107);
    *(SpriteResource **)((u8 *)var_r5 + 0x1E0) = AddPlttResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x130), NARC_poketool_icongra_poke_icon, (s32)sub_02074490(), 0, 0xA, 1, 3, HEAP_ID_107);
    *(SpriteResource **)((u8 *)var_r5 + 0x1E4) = AddCellOrAnimResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x134), temp_r7_2, (s32)sub_02074498(), 0, 5, GF_GFX_RES_TYPE_CELL, HEAP_ID_107);
    *(SpriteResource **)((u8 *)var_r5 + 0x1E8) = AddCellOrAnimResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x138), temp_r7_2, (s32)sub_020744A4(), 0, 5, GF_GFX_RES_TYPE_ANIM, HEAP_ID_107);
    var_r4_3 = 0;
    var_r6_3 = var_r5;
    do {
        if (var_r4_3 == 3) {
            if (sp18 == 0) {
                var_r0 = Party_GetMonByIndex(sp14, 0);
            } else {
                var_r0 = Party_GetMonByIndex(sp14, var_r4_3);
            }
        } else {
            var_r0 = Party_GetMonByIndex(sp14, var_r4_3);
        }
        *(SpriteResource **)((u8 *)var_r6_3 + offsetof(Ov83App_02246E08, unk_13C[10].unk_0)) = AddCharResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x12C), temp_r7_2, (s32)Pokemon_GetIconNaix(var_r0), 0, var_r4_3 + 0xA, 1, HEAP_ID_107);
        var_r4_3 += 1;
        var_r6_3 += sizeof(((Ov83App_02246E08 *)0)->unk_13C[10]);
    } while (var_r4_3 < 4);
    NARC_Delete(temp_r7_2);
    var_r6_4 = 0;
    var_r4_4 = var_r5;
    do {
        SpriteTransfer_CreateCharTransferTask(*(SpriteResource **)((u8 *)var_r4_4 + offsetof(Ov83App_02246E08, unk_13C[0].unk_0)));
        var_r6_4 += 1;
        var_r4_4 += sizeof(((Ov83App_02246E08 *)0)->unk_13C[0]);
    } while (var_r6_4 < 0xE);
    var_r4_5 = 0;
    do {
        SpriteTransfer_CreateExtPlttTransferTask(*(SpriteResource **)((u8 *)var_r5 + 0x140));
        var_r4_5 += 1;
        var_r5 += 0x10;
    } while (var_r4_5 < 0xB);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
}
