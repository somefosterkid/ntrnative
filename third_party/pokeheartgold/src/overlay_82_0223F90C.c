#include "global.h"

#include "overlay_82.h"
#include "render_window.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "pokemon_icon_idx.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "overlay_82_battle_hall_app.h"

s32 ov82_0223EFCC(void *, void *, s32, s32, u8, u8, u8, u8);

s32 ov82_0223FCB0(void *, s32);

void ov82_0223F90C(void *arg0);

s32 ov82_0223FC14(void);
extern u8 ov82_0223FEEC;
void ov82_0223F95C(void *arg0, Pokemon *arg1);

void ov82_0223F90C(void *arg0) {
    ov82_0223FCB0(*((void **)&((BattleHallApp *)arg0)->monSprite), 1);
    ClearFrameAndWindow2(arg0 + 0x4C, 0);
    ov82_0223EFCC(arg0, arg0 + 0x5C, 0, 0, 1, 2, 0, 0);
}

void ov82_0223F948(s32 brightness) {
    G2_SetBlendBrightness(0x1E, brightness);
}

void ov82_0223F95C(void *arg0, Pokemon *arg1) {
    NARC *narc;
    Pokemon *mon;
    GfGfxResType type;
    s32 v1;
    u8 *v0;
    void *v2;
    void *v3;

    v2 = arg0;
    mon = arg1;
    ov82_0223FC14();
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x80, 0, 0x20, 0, 0x80, 0, 0x20, HEAP_ID_105);
    *(SpriteList **)v2 = G2dRenderer_Init(2, v2 + 4, HEAP_ID_105);
    v0 = &ov82_0223FEEC;
    type = GF_GFX_RES_TYPE_CHAR;
    v3 = v2;
    do {
        *(GF_2DGfxResMan **)((u8 *)v3 + 0x12C) = Create2DGfxResObjMan((s32)*v0, type, HEAP_ID_105);
        v0 += 1;
        v3 += 4;
        type += 1;
    } while (type < 4);
    *(SpriteResource **)((u8 *)v2 + 0x13C) = AddCharResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)v2 + 0x12C), NARC_a_1_8_4, 0xC, 1, 0, 1, HEAP_ID_105);
    *(SpriteResource **)((u8 *)v2 + 0x140) = AddPlttResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)v2 + 0x130), NARC_a_1_8_4, 0x36, 0, 0, 1, 1, HEAP_ID_105);
    *(SpriteResource **)((u8 *)v2 + 0x144) = AddCellOrAnimResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)v2 + 0x134), NARC_a_1_8_4, 0xE, 1, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_105);
    *(SpriteResource **)((u8 *)v2 + 0x148) = AddCellOrAnimResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)v2 + 0x138), NARC_a_1_8_4, 0xD, 1, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_105);
    narc = NARC_New(NARC_poketool_icongra_poke_icon, HEAP_ID_105);
    *(SpriteResource **)((u8 *)v2 + 0x14C) = AddCharResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)v2 + 0x12C), narc, (s32)Pokemon_GetIconNaix(mon), 0, 1, 1, HEAP_ID_105);
    *(SpriteResource **)((u8 *)v2 + 0x150) = AddPlttResObjFromNarc(*(GF_2DGfxResMan **)((u8 *)v2 + 0x130), NARC_poketool_icongra_poke_icon, (s32)sub_02074490(), 0, 1, 1, 3, HEAP_ID_105);
    *(SpriteResource **)((u8 *)v2 + 0x154) = AddCellOrAnimResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)v2 + 0x134), narc, (s32)sub_02074498(), 0, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_105);
    *(SpriteResource **)((u8 *)v2 + 0x158) = AddCellOrAnimResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)v2 + 0x138), narc, (s32)sub_020744A4(), 0, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_105);
    v1 = 0;
    do {
        SpriteTransfer_CreateCharTransferTask(*(SpriteResource **)((u8 *)v2 + 0x13C));
        SpriteTransfer_CreateExtPlttTransferTask(*(SpriteResource **)((u8 *)v2 + 0x140));
        v1 += 1;
        v2 += 0x10;
    } while (v1 < 2);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    NARC_Delete(narc);
}
