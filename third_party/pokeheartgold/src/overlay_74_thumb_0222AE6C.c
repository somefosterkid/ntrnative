#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_74_mystery_gift_templates.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "screen_fade.h"
#include "sound_02004A44.h"
#include "system.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_74_mystery_gift_animation.h"
#include "overlay_74_mystery_gift_app_data.h"
#include "main.h"


s32 ov74_0222A744(BgConfig *);

void ov74_0222CD88(struct OverlayManager *man);

s32 ov74_0222FCA4(void);

s32 ov74_0223512C(s32);

s32 ov74_02235230(void);

s32 ov74_02236034(s32);

void ov74_0222AF28(MysteryGiftAnimationManager *animMan);

BOOL ov74_0222AE6C(OverlayManager *man, int *state) {
    SaveData *saveData;
    MysteryGiftAppData *v0;

    ov74_0222CD88(man);
    Heap_Create(HEAP_ID_3, HEAP_ID_84, 0x30000U);
    v0 = OverlayManager_CreateAndGetData(man, sizeof(MysteryGiftAppData), HEAP_ID_84);
    memset(v0, 0, sizeof(MysteryGiftAppData));
    v0->bgConfig = BgConfig_Alloc(HEAP_ID_84);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    ov74_0222FCA4();
    ov74_0222A744(v0->bgConfig);
    sub_0200FBF4(PM_LCD_TOP, 0U);
    sub_0200FBF4(PM_LCD_BOTTOM, 0U);
    Sound_SetSceneAndPlayBGM(0xAU, 0x47EU, 1);
    ov74_0223512C(0x54);
    if (ov74_02235230() != 0) {
        v0->receptionMethod = 2;
        sub_0201A4B0((s32)gGameVersion);
        ov74_02236034(1);
    }
    v0->unk_5C8 = 0x1D;
    saveData = ((UnkStruct_02111868_sub *)OverlayManager_GetArgs(man))->saveData;
    v0->saveData = saveData;
    v0->options = Save_PlayerData_GetOptionsAddr(saveData);
    v0->msgBoxPrinterDelay = 0xFF;
    Heap_Create(HEAP_ID_DEFAULT, HEAP_ID_89, 0x570U);
    return 1;
}

void ov74_0222AF28(MysteryGiftAnimationManager *animMan) {
    SpriteResourcesHeader *headers;

    animMan->spriteResources[0][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(animMan->spriteMan->resourceCollections[GF_GFX_RES_TYPE_CHAR], NARC_a_1_1_3, 0x2B, 1, 0x4E21, 1, HEAP_ID_84);
    animMan->spriteResources[0][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(animMan->spriteMan->resourceCollections[GF_GFX_RES_TYPE_PLTT], NARC_a_1_1_3, 0x28, 0, 0x4E21, 1, 1, HEAP_ID_84);
    animMan->spriteResources[0][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(animMan->spriteMan->resourceCollections[GF_GFX_RES_TYPE_CELL], NARC_a_1_1_3, 0x2A, 1, 0x4E21, GF_GFX_RES_TYPE_CELL, HEAP_ID_84);
    animMan->spriteResources[0][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(animMan->spriteMan->resourceCollections[GF_GFX_RES_TYPE_ANIM], NARC_a_1_1_3, 0x29, 1, 0x4E21, GF_GFX_RES_TYPE_ANIM, HEAP_ID_84);
    animMan->spriteResources[1][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(animMan->spriteMan->resourceCollections[GF_GFX_RES_TYPE_CHAR], NARC_a_1_1_3, 0x2B, 1, 0x4E22, 2, HEAP_ID_84);
    animMan->spriteResources[1][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(animMan->spriteMan->resourceCollections[GF_GFX_RES_TYPE_PLTT], NARC_a_1_1_3, 0x28, 0, 0x4E22, 2, 1, HEAP_ID_84);
    animMan->spriteResources[1][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(animMan->spriteMan->resourceCollections[GF_GFX_RES_TYPE_CELL], NARC_a_1_1_3, 0x2A, 1, 0x4E22, GF_GFX_RES_TYPE_CELL, HEAP_ID_84);
    animMan->spriteResources[1][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(animMan->spriteMan->resourceCollections[GF_GFX_RES_TYPE_ANIM], NARC_a_1_1_3, 0x29, 1, 0x4E22, GF_GFX_RES_TYPE_ANIM, HEAP_ID_84);
    SpriteTransfer_CreateCharTransferTask(animMan->spriteResources[0][GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_CreateCharTransferTask(animMan->spriteResources[1][GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_CreatePlttTransferTask(animMan->spriteResources[0][GF_GFX_RES_TYPE_PLTT]);
    SpriteTransfer_CreatePlttTransferTask(animMan->spriteResources[1][GF_GFX_RES_TYPE_PLTT]);
    CreateSpriteResourcesHeader(&animMan->spriteResourcesHeaders[0], 0x4E21, 0x4E21, 0x4E21, 0x4E21, -1, -1, 0, 0, animMan->spriteMan->resourceCollections[GF_GFX_RES_TYPE_CHAR], animMan->spriteMan->resourceCollections[GF_GFX_RES_TYPE_PLTT], animMan->spriteMan->resourceCollections[GF_GFX_RES_TYPE_CELL], animMan->spriteMan->resourceCollections[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    headers = animMan->spriteResourcesHeaders;
    CreateSpriteResourcesHeader(&headers[1], 0x4E22, 0x4E22, 0x4E22, 0x4E22, -1, -1, 0, 0, animMan->spriteMan->resourceCollections[GF_GFX_RES_TYPE_CHAR], animMan->spriteMan->resourceCollections[GF_GFX_RES_TYPE_PLTT], animMan->spriteMan->resourceCollections[GF_GFX_RES_TYPE_CELL], animMan->spriteMan->resourceCollections[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
}
