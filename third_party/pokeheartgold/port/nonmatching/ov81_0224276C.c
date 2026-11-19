#include "gf_gfx_planes.h"
#include "pokemon_icon_idx.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
void ov81_02242B90(void);
extern u8 ov81_02243594[];

void ov81_0224276C(void *arg0) {
    s32 i;
    void *var_r5;

    var_r5 = arg0;
    ov81_02242B90();
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x80, 0, 0x20, 0, 0x80, 0, 0x20, HEAP_ID_100);
    *(SpriteList **)var_r5 = G2dRenderer_Init(0x20, (void *)((u8 *)var_r5 + 4), HEAP_ID_100);
    G2dRenderer_SetSubSurfaceCoords((void *)((u8 *)var_r5 + 4), 0, FX32_CONST(512));
    for (i = 0; i < 4; i++) {
        *(GF_2DGfxResMan **)((u8 *)var_r5 + i * 4 + 0x12C) = Create2DGfxResObjMan(ov81_02243594[i], (GfGfxResType)i, HEAP_ID_100);
    }
    *(SpriteResource **)((u8 *)var_r5 + 0x14C) = AddCharResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x12C), NARC_a_1_8_4, 0x28, 1, 1, 1, HEAP_ID_100);
    *(SpriteResource **)((u8 *)var_r5 + 0x150) = AddPlttResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x130), NARC_a_1_8_4, 0x3E, 0, 1, 1, 4, HEAP_ID_100);
    *(SpriteResource **)((u8 *)var_r5 + 0x154) = AddCellOrAnimResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x134), NARC_a_1_8_4, 0x29, 1, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_100);
    *(SpriteResource **)((u8 *)var_r5 + 0x158) = AddCellOrAnimResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x138), NARC_a_1_8_4, 0x2A, 1, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_100);
    *(SpriteResource **)((u8 *)var_r5 + 0x15C) = AddCharResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x12C), NARC_a_1_8_4, 0x2B, 1, 2, 1, HEAP_ID_100);
    *(SpriteResource **)((u8 *)var_r5 + 0x160) = AddPlttResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x130), NARC_a_1_8_4, 0x3F, 0, 2, 1, 1, HEAP_ID_100);
    *(SpriteResource **)((u8 *)var_r5 + 0x164) = AddCellOrAnimResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x134), NARC_a_1_8_4, 0x2C, 1, 2, GF_GFX_RES_TYPE_CELL, HEAP_ID_100);
    *(SpriteResource **)((u8 *)var_r5 + 0x168) = AddCellOrAnimResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x138), NARC_a_1_8_4, 0x2D, 1, 2, GF_GFX_RES_TYPE_ANIM, HEAP_ID_100);
    *(SpriteResource **)((u8 *)var_r5 + 0x13C) = AddCharResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x12C), NARC_a_1_8_4, 0, 1, 0, 2, HEAP_ID_100);
    *(SpriteResource **)((u8 *)var_r5 + 0x140) = AddPlttResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x130), NARC_a_1_8_4, 0x34, 0, 0, 2, 8, HEAP_ID_100);
    *(SpriteResource **)((u8 *)var_r5 + 0x144) = AddCellOrAnimResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x134), NARC_a_1_8_4, 2, 1, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_100);
    *(SpriteResource **)((u8 *)var_r5 + 0x148) = AddCellOrAnimResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x138), NARC_a_1_8_4, 1, 1, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_100);
    *(SpriteResource **)((u8 *)var_r5 + 0x16C) = AddCharResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x12C), NARC_a_1_8_4, 0x2B, 1, 3, 2, HEAP_ID_100);
    *(SpriteResource **)((u8 *)var_r5 + 0x170) = AddPlttResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x130), NARC_a_1_8_4, 0x3F, 0, 3, 2, 1, HEAP_ID_100);
    *(SpriteResource **)((u8 *)var_r5 + 0x174) = AddCellOrAnimResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x134), NARC_a_1_8_4, 0x2C, 1, 3, GF_GFX_RES_TYPE_CELL, HEAP_ID_100);
    *(SpriteResource **)((u8 *)var_r5 + 0x178) = AddCellOrAnimResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x138), NARC_a_1_8_4, 0x2D, 1, 3, GF_GFX_RES_TYPE_ANIM, HEAP_ID_100);
    for (i = 0; i < 2; i++) {
        *(SpriteResource **)((u8 *)var_r5 + i * 0x10 + 0x17C) = AddCharResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x12C), NARC_a_1_8_4, 0x2E, 1, i + 4, 2, HEAP_ID_100);
        *(SpriteResource **)((u8 *)var_r5 + i * 0x10 + 0x180) = AddPlttResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x130), NARC_poketool_icongra_poke_icon, (s32) sub_02074490(), 0, i + 4, 2, 3, HEAP_ID_100);
        *(SpriteResource **)((u8 *)var_r5 + i * 0x10 + 0x184) = AddCellOrAnimResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x134), NARC_a_1_8_4, 0x2F, 1, i + 4, GF_GFX_RES_TYPE_CELL, HEAP_ID_100);
        *(SpriteResource **)((u8 *)var_r5 + i * 0x10 + 0x188) = AddCellOrAnimResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)var_r5 + 0x138), NARC_a_1_8_4, 0x30, 1, i + 4, GF_GFX_RES_TYPE_ANIM, HEAP_ID_100);
    }
    for (i = 0; i < 6; i++) {
        SpriteTransfer_CreateCharTransferTask(*(SpriteResource **)((u8 *)var_r5 + i * 0x10 + 0x13C));
        SpriteTransfer_CreateExtPlttTransferTask(*(SpriteResource **)((u8 *)var_r5 + i * 0x10 + 0x140));
    }
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
}
