#include "global.h"

#include "overlay_70_gtsapplication_state.h"

#include "bg_window.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "render_window.h"
#include "sprite.h"

s32 ov70_022392BC(BgConfig *bgConfig);

void ov70_0223CF48(BgConfig *arg0);

void ov70_02239C6C(GTSApplicationState *);
void ov70_02239CF8(GTSApplicationState *);
void ov70_0223CF74(GTSApplicationState *param_1);

void ov70_02238B54(SpriteTemplate *, GTSApplicationState *, SpriteResourcesHeader *, u32);
void ov70_02238F9C(Sprite *, u32, u32);
extern u32 ov70_022456E8;
extern u32 ov70_02245784;
extern u32 ov70_02245786;
void ov70_0223D058(GTSApplicationState *param_1);

void ov70_0223CF48(BgConfig *bgConfig) {
    ov70_022392BC(bgConfig);
    FreeBgTilemapBuffer(bgConfig, 2U);
    FreeBgTilemapBuffer(bgConfig, 1U);
    FreeBgTilemapBuffer(bgConfig, 0U);
    FreeBgTilemapBuffer(bgConfig, 3U);
}

void ov70_0223CF74(GTSApplicationState *param_1) {
    NARC *narc;
    u32 uVar1;
    BgConfig *bgConfig;

    bgConfig = (BgConfig *)param_1->bgConfig;
    narc = NARC_New(NARC_a_1_0_0, HEAP_ID_61);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 2, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x60, HEAP_ID_61);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 5, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x100, HEAP_ID_61);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_61);
    uVar1 = Options_GetFrame(*(Options **)((u8 *)param_1->playerData + 0x24));
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 1, 0xe, (u8)uVar1, HEAP_ID_61);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 0x1f, 0xb, 0, HEAP_ID_61);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0xd, bgConfig, GF_BG_LYR_MAIN_1, 0, 0xa00, 1, HEAP_ID_61);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x1b, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, 1, HEAP_ID_61);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x26, bgConfig, GF_BG_LYR_MAIN_2, 0, 0x600, 1, HEAP_ID_61);
    ov70_02239C6C(param_1);
    ov70_02239CF8(param_1);
    NARC_Delete(narc);
    return;
}

void ov70_0223D058(GTSApplicationState *param_1) {
    Sprite *pSVar1;
    u16 *puVar2;
    s32 iVar4;
#if !(defined(SDK_PORT) || defined(NONMATCHING))
    u8 *iVar3;
#endif
    SpriteTemplate SStack_48;
    ov70_02238B54(&SStack_48, param_1, &param_1->cursorSpriteResourceHeader, 1);
    SStack_48.position.x = ((u32)(*((u16 *)((&ov70_02245784) + ((u32)(param_1->partySlotIndex)))))) << 0xc;
    SStack_48.position.y = ((u32)(*((u16 *)((&ov70_02245786) + ((u32)(param_1->partySlotIndex)))))) << 0xc;
    pSVar1 = Sprite_CreateAffine(&SStack_48);
    param_1->cursorSprite = pSVar1;
    Sprite_SetAnimActiveFlag(param_1->cursorSprite, 1);
    Sprite_SetAnimCtrlSeq(param_1->cursorSprite, 4);
    if (((param_1->partySlotIndex) == 0x1f) || ((param_1->partySlotIndex) <= (6 - 1))) {
        Sprite_SetPriority(param_1->cursorSprite, 0);
    } else {
        Sprite_SetPriority(param_1->cursorSprite, 1);
    }
#if defined(SDK_PORT) || defined(NONMATCHING)
    puVar2 = (u16 *)(&ov70_02245784);
    iVar4 = 0;
    do {
        SStack_48.position.x = ((u32)(*puVar2)) << 0xc;
        SStack_48.position.y = ((u32)puVar2[1]) << 0xc;
        SStack_48.drawPriority = 0x14;
        pSVar1 = Sprite_CreateAffine(&SStack_48);
        param_1->boxSlotIconSprites[iVar4] = pSVar1;
        Sprite_SetAnimCtrlSeq(param_1->boxSlotIconSprites[iVar4], iVar4 + 6);
        Sprite_SetPriority(param_1->boxSlotIconSprites[iVar4], 1);
        puVar2 = puVar2 + 2;
        iVar4 = iVar4 + 1;
    } while (iVar4 < 0x1e);
    puVar2 = (u16 *)(&ov70_02245784);
    iVar4 = 0;
    do {
        SStack_48.position.x = ((u32)(*puVar2)) << 0xc;
        SStack_48.position.y = (puVar2[1] + 6) * 0x1000;
        SStack_48.drawPriority = 10;
        pSVar1 = Sprite_CreateAffine(&SStack_48);
        param_1->boxSlotItemSprites[iVar4] = pSVar1;
        Sprite_SetAnimCtrlSeq(param_1->boxSlotItemSprites[iVar4], 0x28);
        Sprite_SetPriority(param_1->boxSlotItemSprites[iVar4], 1);
        iVar4 = iVar4 + 1;
        puVar2 = puVar2 + 2;
    } while (iVar4 < 0x1e);
    puVar2 = (u16 *)(&ov70_02245784);
    iVar4 = 0;
    do {
        SStack_48.position.x = ((*puVar2) + 8) * 0x1000;
        SStack_48.position.y = (puVar2[1] + 6) * 0x1000;
        SStack_48.drawPriority = 10;
        pSVar1 = Sprite_CreateAffine(&SStack_48);
        param_1->selectPokemonPartySprites[iVar4] = pSVar1;
        Sprite_SetAnimCtrlSeq(param_1->selectPokemonPartySprites[iVar4], 0x2a);
        Sprite_SetPriority(param_1->selectPokemonPartySprites[iVar4], 1);
        iVar4 = iVar4 + 1;
        puVar2 = puVar2 + 2;
    } while (iVar4 < 6);
    puVar2 = (u16 *)(&ov70_022456E8);
    iVar4 = 0;
    do {
        SStack_48.position.x = ((u32)(*puVar2)) << 0xc;
        SStack_48.position.y = ((u32)puVar2[1]) << 0xc;
        pSVar1 = Sprite_CreateAffine(&SStack_48);
        param_1->boxArrowSprites[iVar4] = pSVar1;
        Sprite_SetAnimCtrlSeq(param_1->boxArrowSprites[iVar4], iVar4 + 0x26);
        Sprite_SetPriority(param_1->boxArrowSprites[iVar4], 1);
        iVar4 = iVar4 + 1;
        puVar2 = puVar2 + 2;
    } while (iVar4 < 2);
#else
    // DS build: byte-pointer walk over the sprite arrays (the indexed form allocates registers differently)
    puVar2 = (u16 *)(&ov70_02245784);
    iVar4 = 0;
    iVar3 = (u8 *)param_1;
    do {
        SStack_48.position.x = ((u32)(*puVar2)) << 0xc;
        SStack_48.position.y = ((u32)puVar2[1]) << 0xc;
        SStack_48.drawPriority = 0x14;
        pSVar1 = Sprite_CreateAffine(&SStack_48);
        *((Sprite **)(iVar3 + 0xdd8)) = pSVar1;
        Sprite_SetAnimCtrlSeq(*((Sprite **)(iVar3 + 0xdd8)), iVar4 + 6);
        Sprite_SetPriority(*((Sprite **)(iVar3 + 0xdd8)), 1);
        puVar2 = puVar2 + 2;
        iVar3 = iVar3 + 4;
        iVar4 = iVar4 + 1;
    } while (iVar4 < 0x1e);
    puVar2 = (u16 *)(&ov70_02245784);
    iVar4 = 0;
    iVar3 = (u8 *)param_1;
    do {
        SStack_48.position.x = ((u32)(*puVar2)) << 0xc;
        SStack_48.position.y = (puVar2[1] + 6) * 0x1000;
        SStack_48.drawPriority = 10;
        pSVar1 = Sprite_CreateAffine(&SStack_48);
        *((Sprite **)(iVar3 + 0xe50)) = pSVar1;
        Sprite_SetAnimCtrlSeq(*((Sprite **)(iVar3 + 0xe50)), 0x28);
        Sprite_SetPriority(*((Sprite **)(iVar3 + 0xe50)), 1);
        iVar4 = iVar4 + 1;
        puVar2 = puVar2 + 2;
        iVar3 = iVar3 + 4;
    } while (iVar4 < 0x1e);
    puVar2 = (u16 *)(&ov70_02245784);
    iVar4 = 0;
    iVar3 = (u8 *)param_1;
    do {
        SStack_48.position.x = ((*puVar2) + 8) * 0x1000;
        SStack_48.position.y = (puVar2[1] + 6) * 0x1000;
        SStack_48.drawPriority = 10;
        pSVar1 = Sprite_CreateAffine(&SStack_48);
        *((Sprite **)(iVar3 + 0xec8)) = pSVar1;
        Sprite_SetAnimCtrlSeq(*((Sprite **)(iVar3 + 0xec8)), 0x2a);
        Sprite_SetPriority(*((Sprite **)(iVar3 + 0xec8)), 1);
        iVar4 = iVar4 + 1;
        puVar2 = puVar2 + 2;
        iVar3 = iVar3 + 4;
    } while (iVar4 < 6);
    puVar2 = (u16 *)(&ov70_022456E8);
    iVar4 = 0;
    iVar3 = (u8 *)param_1;
    do {
        SStack_48.position.x = ((u32)(*puVar2)) << 0xc;
        SStack_48.position.y = ((u32)puVar2[1]) << 0xc;
        pSVar1 = Sprite_CreateAffine(&SStack_48);
        *((Sprite **)(iVar3 + 0xf04)) = pSVar1;
        Sprite_SetAnimCtrlSeq(*((Sprite **)(iVar3 + 0xf04)), iVar4 + 0x26);
        Sprite_SetPriority(*((Sprite **)(iVar3 + 0xf04)), 1);
        iVar4 = iVar4 + 1;
        puVar2 = puVar2 + 2;
        iVar3 = iVar3 + 4;
    } while (iVar4 < 2);
#endif
    Sprite_SetDrawFlag(param_1->unk_F10, 1);
    ov70_02238F9C(param_1->unk_F10, 0x37, 0x1a8);
    return;
}
