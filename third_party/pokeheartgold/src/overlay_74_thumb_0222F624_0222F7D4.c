#include "global.h"

#include "constants/heap.h"
#include "constants/sndseq.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "main.h"
#include "mystery_gift.h"
#include "options.h"
#include "overlay_74_app_templates.h"
#include "overlay_74_wonder_cards_app2_data.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "render_window.h"
#include "save.h"
#include "save_arrays.h"
#include "screen_fade.h"
#include "sprite.h"
#include "system.h"
#include "text.h"
#include "unk_02005D10.h"
#include "unk_02037C94.h"

void ov74_0222EC60(WonderCardsApp2Data *appData);

void ov74_0222ECD4(Window *window, u8 skipTransfer);

void ov74_0222ECEC(Window *window, u8 skipTransfer);

void ov74_0222EDC0(WonderCardsApp2Data *appData);

void ov74_0222EFF0(WonderCardsApp2Data *appData, enum FadeType fadeType, int nextState, int *state);

void ov74_0222F024(WonderCardsApp2Data *appData, BOOL unused, int screen);

int ov74_0222F1BC(WonderCardsApp2Data *appData, Window *window, int windowTemplateIdx, int baseTile);

void ov74_0222F314(WonderCardsApp2Data *appData, int screen);

void ov74_0222F404(OverlayManager *man, int *state, WonderCardsApp2StateFunc onCancel);

void ov74_0222F624(WonderCardsApp2Data *appData, int stage, fx32 startOffsetStep, fx32 startDOfsStep);

void ov74_0222F688(WonderCardsApp2Data *appData);

BOOL ov74_0222F6C4(WonderCardsApp2Data *appData);

void ov74_0222FCA4(void);

void ov74_0222FCC4(BgConfig *bgConfig);

void ov74_022358BC(void);

BOOL ov74_0222F7D4(OverlayManager *man, int *state) {
    WonderCardsApp2Data *appData = OverlayManager_GetData(man);
    UnkStruct_02111868_sub *args;

    switch (*state) {
    case 0:
        args = OverlayManager_GetArgs(man);
        appData->saveData = args->saveData;
        appData->mysteryGift = Save_MysteryGift_Get(appData->saveData);
        appData->options = Save_PlayerData_GetOptionsAddr(appData->saveData);
        appData->msgBoxFrame = Options_GetFrame(appData->options);
        appData->wonderCard = SaveMysteryGift_CardGetByIdx(appData->mysteryGift, 4);
        *state = 1;
        break;
    case 1:
        ov74_0222FCA4();
        ov74_0222FCC4(appData->bgConfig);
        *state = 2;
        break;
    case 2:
        ResetAllTextPrinters();
        ov74_0222F314(appData, 0);
        LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, HEAP_ID_85);
        LoadUserFrameGfx1(appData->bgConfig, GF_BG_LYR_MAIN_0, 1, 13, 0, HEAP_ID_85);
        LoadUserFrameGfx1(appData->bgConfig, GF_BG_LYR_MAIN_0, 10, 14, 1, HEAP_ID_85);
        LoadUserFrameGfx2(appData->bgConfig, GF_BG_LYR_MAIN_0, 0x13, 10, appData->msgBoxFrame, HEAP_ID_85);
        ov74_0222F024(appData, TRUE, 0);
        ov74_0222EFF0(appData, FADE_TYPE_BRIGHTNESS_IN, 3, state);
        break;
    case 3:
        if (gSystem.newKeys & PAD_BUTTON_B) {
            PlaySE(SEQ_SE_DP_SELECT);
            ov74_0222EFF0(appData, FADE_TYPE_BRIGHTNESS_OUT, 0x1D, state);
        } else if (gSystem.newKeys & PAD_BUTTON_A) {
            PlaySE(SEQ_SE_DP_SELECT);
            *state = 4;
        }
        break;
    case 4:
        appData->nextWindowBasetile = ov74_0222F1BC(appData, &appData->messageBox, 7, 0x280);
        ov74_0222F1BC(appData, &appData->standardWindow, 8, appData->nextWindowBasetile);
        *state = 5;
        break;
    case 5:
        ov74_0222F404(man, state, NULL);
        if (gSystem.newKeys & PAD_BUTTON_B) {
            PlaySE(SEQ_SE_DP_SELECT);
            *state = 11;
        }
        break;
    case 6:
        PlaySE(SEQ_SE_DP_CARD2);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
        ov74_0222ECEC(&appData->messageBox, FALSE);
        ov74_0222ECD4(&appData->standardWindow, FALSE);
        ov74_0222F624(appData, 1, FX32_ONE, 0x66);
        *state = 7;
        break;
    case 7:
        if (ov74_0222F6C4(appData)) {
            ov74_0222F024(appData, TRUE, 1);
            ov74_0222F624(appData, 0, 1800 * FX32_ONE, 900 * FX32_ONE);
            *state = 8;
        }
        break;
    case 8:
        ov74_0222F6C4(appData);
        if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            PlaySE(SEQ_SE_DP_CARD2);
            ov74_0222F624(appData, 1, FX32_ONE, 0x66);
            *state = 9;
        }
        break;
    case 9:
        if (ov74_0222F6C4(appData)) {
            ov74_0222F024(appData, TRUE, 0);
            ov74_0222F624(appData, 0, 1800 * FX32_ONE, 900 * FX32_ONE);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
            *state = 10;
        }
        break;
    case 10:
        if (ov74_0222F6C4(appData)) {
            DrawFrameAndWindow2(&appData->messageBox, FALSE, 0x13, 10);
            DrawFrameAndWindow1(&appData->standardWindow, FALSE, 10, 14);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
            ov74_0222F688(appData);
            *state = 5;
        }
        break;
    case 11:
        ov74_0222EDC0(appData);
        ov74_0222ECEC(&appData->messageBox, FALSE);
        ClearWindowTilemapAndCopyToVram(&appData->messageBox);
        RemoveWindow(&appData->messageBox);
        *state = 3;
        break;
    case 20:
        ov74_0222EFF0(appData, FADE_TYPE_BRIGHTNESS_OUT, 0x15, state);
        break;
    case 21:
        ov74_0222EC60(appData);
        ov74_0222ECEC(&appData->messageBox, FALSE);
        ov74_0222ECD4(&appData->standardWindow, FALSE);
        ov74_0222F024(appData, TRUE, 0);
        LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, HEAP_ID_85);
        ov74_0222EFF0(appData, FADE_TYPE_BRIGHTNESS_IN, 4, state);
        break;
    case 23:
        if (!sub_02037D78()) {
            *state = appData->queuedState;
        }
        break;
    case 13:
        if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            ov74_0222ECEC(&appData->messageBox, FALSE);
            ClearWindowTilemapAndCopyToVram(&appData->messageBox);
            RemoveWindow(&appData->messageBox);
            *state = 3;
        }
        break;
    case 27:
        ov74_0222EFF0(appData, FADE_TYPE_BRIGHTNESS_OUT, 0x1D, state);
        break;
    case 28:
        if (IsPaletteFadeFinished()) {
            *state = appData->queuedState;
        }
        break;
    case 29:
        ov74_0222EC60(appData);
        ov74_0222F688(appData);
        return TRUE;
    case 12:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 22:
    case 24:
    case 25:
    case 26:
    case 30:
        break;
    }

    if (appData->spriteList != NULL) {
        SpriteList_RenderAndAnimateSprites(appData->spriteList);
    }
    ov74_022358BC();
    if (appData->mainCallback != NULL) {
        appData->mainCallback(appData);
    }
    return FALSE;
}
