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

s32 ov82_0223EFCC(void *, Window *, s32, s32, u8, u8, u8, u8);

s32 ov82_0223FCB0(void *, s32);

void ov82_0223F90C(void *arg0);

s32 ov82_0223FC14(void);
extern u8 ov82_0223FEEC;
void ov82_0223F95C(BattleHallAppSpriteManager *spriteMan, Pokemon *arg1);

void ov82_0223F90C(void *arg0) {
    ov82_0223FCB0(*((void **)&((BattleHallApp *)arg0)->monSprite), 1);
    ClearFrameAndWindow2(&((BattleHallApp *)arg0)->windows[0], 0);
    ov82_0223EFCC(arg0, &((BattleHallApp *)arg0)->windows[1], 0, 0, 1, 2, 0, 0);
}

void ov82_0223F948(s32 brightness) {
    G2_SetBlendBrightness(0x1E, brightness);
}

void ov82_0223F95C(BattleHallAppSpriteManager *spriteMan, Pokemon *arg1) {
    NARC *narc;
    Pokemon *mon;
    GfGfxResType type;
    s32 v1;
    u8 *v0;
#if !(defined(SDK_PORT) || defined(NONMATCHING))
    void *v3;
#endif

    mon = arg1;
    ov82_0223FC14();
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x80, 0, 0x20, 0, 0x80, 0, 0x20, HEAP_ID_105);
    spriteMan->spriteList = G2dRenderer_Init(2, &spriteMan->renderer, HEAP_ID_105);
    v0 = &ov82_0223FEEC;
    type = GF_GFX_RES_TYPE_CHAR;
#if defined(SDK_PORT) || defined(NONMATCHING)
    do {
        spriteMan->resourceCollection[type] = Create2DGfxResObjMan((s32)*v0, type, HEAP_ID_105);
        v0 += 1;
        type += 1;
    } while (type < 4);
#else
    v3 = (void *)spriteMan;
    do {
        *(GF_2DGfxResMan **)((u8 *)v3 + 0x12C) = Create2DGfxResObjMan((s32)*v0, type, HEAP_ID_105);
        v0 += 1;
        v3 += 4;
        type += 1;
    } while (type < 4);
#endif
    spriteMan->resources[0][0] = AddCharResObjFromNarc(spriteMan->resourceCollection[0], NARC_a_1_8_4, 0xC, 1, 0, 1, HEAP_ID_105);
    spriteMan->resources[0][1] = AddPlttResObjFromNarc(spriteMan->resourceCollection[1], NARC_a_1_8_4, 0x36, 0, 0, 1, 1, HEAP_ID_105);
    spriteMan->resources[0][2] = AddCellOrAnimResObjFromNarc(spriteMan->resourceCollection[2], NARC_a_1_8_4, 0xE, 1, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_105);
    spriteMan->resources[0][3] = AddCellOrAnimResObjFromNarc(spriteMan->resourceCollection[3], NARC_a_1_8_4, 0xD, 1, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_105);
    narc = NARC_New(NARC_poketool_icongra_poke_icon, HEAP_ID_105);
    spriteMan->resources[1][0] = AddCharResObjFromOpenNarc(spriteMan->resourceCollection[0], narc, (s32)Pokemon_GetIconNaix(mon), 0, 1, 1, HEAP_ID_105);
    spriteMan->resources[1][1] = AddPlttResObjFromNarc(spriteMan->resourceCollection[1], NARC_poketool_icongra_poke_icon, (s32)sub_02074490(), 0, 1, 1, 3, HEAP_ID_105);
    spriteMan->resources[1][2] = AddCellOrAnimResObjFromOpenNarc(spriteMan->resourceCollection[2], narc, (s32)sub_02074498(), 0, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_105);
    spriteMan->resources[1][3] = AddCellOrAnimResObjFromOpenNarc(spriteMan->resourceCollection[3], narc, (s32)sub_020744A4(), 0, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_105);
    for (v1 = 0; v1 < 2; v1++) {
        SpriteTransfer_CreateCharTransferTask(spriteMan->resources[v1][0]);
        SpriteTransfer_CreateExtPlttTransferTask(spriteMan->resources[v1][1]);
    }
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    NARC_Delete(narc);
}
