#include "global.h"

#include "constants/heap.h"
#include "constants/sndseq.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "mystery_gift.h"
#include "options.h"
#include "overlay_74_wonder_cards_app_data.h"
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
#include "unk_02009D48.h"
#include "unk_02035900.h"
#include "unk_020379A0.h"
#include "unk_02037C94.h"
#include "unk_0203A3B0.h"
#include "main.h"

void ov74_02229D0C(const void *param0, int param1);
void ov74_0222CFFC(WonderCardsAppData *appData);
void ov74_0222D024(WonderCardsAppData *appData);
void ov74_0222D098(WonderCardsAppData *appData);
void ov74_0222D0EC(Window *window, u8 skipTransfer);
void ov74_0222D104(Window *window, u8 skipTransfer);
void ov74_0222D248(BgConfig *bgConfig);
void ov74_0222D448(WonderCardsAppData *appData);
BOOL ov74_0222D7A4(WonderCardsAppData *appData, Window *window, u32 color);
void ov74_0222D7F0(WonderCardsAppData *appData, enum FadeType fadeType, enum WonderCardsAppState nextState, int *state);
void ov74_0222D824(WonderCardsAppData *appData, BOOL unused, enum WonderCardsAppScreen screen);
int ov74_0222D9E0(WonderCardsAppData *appData, Window *window, int windowTemplateIdx, int baseTile);
int ov74_0222DAF8(WonderCardsAppData *appData, int current, int direction);
void ov74_0222DB70(WonderCardsAppData *appData, enum WonderCardsAppScreen screen);
void ov74_0222DC60(OverlayManager *appMan, int *state, StateTransitionFuncPtr onCancelStateTransition);
void ov74_0222DE8C(WonderCardsAppData *appData, enum WonderCardFlipStage stage, fx32 startOffsetStep, fx32 startDeltaOffsetStep);
void ov74_0222DEF0(WonderCardsAppData *appData);
BOOL ov74_0222DF2C(WonderCardsAppData *appData);
void ov74_0222E060(WonderCardsAppData *appData);
void ov74_0222E0D4(WonderCardsAppData *appData, int playerCount, int *state);
int ov74_0222E7EC(WonderCardsAppData *appData);
int ov74_0222E898(void);
int ov74_0222E8B4(WonderCardsAppData *appData, Window *window);
void ov74_0222EA88(WonderCardsAppData *appData, Window *window, s32 count);
void ov74_0222EB28(WonderCardsAppData *appData, int *state, enum WonderCardsAppState nextState);
void ov74_0222FCA4(void);
void ov74_0222FCC4(BgConfig *bgConfig);
void ov74_022358BC(void);
void ov74_02235ED0(MysteryGiftEventData *eventData, WonderCard *wonderCard, enum HeapID heapID);
void ov74_02236128(void);
int ov74_0223615C(void);
void sub_020358B8(void *src);
enum WonderCardsAppState ov74_0222D414(OverlayManager *man);

BOOL ov74_0222E1F4(OverlayManager *man, int *state);

BOOL ov74_0222E1F4(OverlayManager *man, int *state) {
    WonderCardsAppData *appData = OverlayManager_GetData(man);
    int selectedWcSlot;
    int connectedPlayersCount;

    switch (*state) {
    case WC_APP_STATE_LOAD_FROM_SAVE:
        appData->saveData = ((UnkStruct_02111868_sub *)OverlayManager_GetArgs(man))->saveData;
        appData->mysteryGift = Save_MysteryGift_Get(appData->saveData);
        appData->options = Save_PlayerData_GetOptionsAddr(appData->saveData);
        appData->msgBoxFrame = Options_GetFrame(appData->options);

        appData->wonderCards[0] = SaveMysteryGift_CardGetByIdx(appData->mysteryGift, 0);
        appData->wonderCards[1] = SaveMysteryGift_CardGetByIdx(appData->mysteryGift, 1);
        appData->wonderCards[2] = SaveMysteryGift_CardGetByIdx(appData->mysteryGift, 2);

        appData->selectedWondercardSlot = ov74_0222DAF8(appData, appData->selectedWondercardSlot, 1);
        *state = WC_APP_STATE_INIT_GRAPHICS;
        break;
    case WC_APP_STATE_INIT_GRAPHICS:
        ov74_0222FCA4();
        ov74_0222FCC4(appData->bgConfig);
        *state = WC_APP_STATE_LOAD_GRAPHICS;
        break;
    case WC_APP_STATE_LOAD_GRAPHICS:
        ResetAllTextPrinters();
        ov74_0222DB70(appData, WC_SCREEN_WONDERCARD_FRONT);
        LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, HEAP_ID_85);
        LoadUserFrameGfx1(appData->bgConfig, GF_BG_LYR_MAIN_0, 1, 0xD, 0, HEAP_ID_85);
        LoadUserFrameGfx1(appData->bgConfig, GF_BG_LYR_MAIN_0, 10, 0xE, 1, HEAP_ID_85);
        LoadUserFrameGfx2(appData->bgConfig, GF_BG_LYR_MAIN_0, 0x13, 0xA, appData->msgBoxFrame, HEAP_ID_85);

        ov74_0222D824(appData, TRUE, WC_SCREEN_WONDERCARD_FRONT);
        ov74_0222D7F0(appData, FADE_TYPE_BRIGHTNESS_IN, WC_APP_STATE_SELECT_WONDERCARD, state);
        break;
    case WC_APP_STATE_SELECT_WONDERCARD:
        selectedWcSlot = appData->selectedWondercardSlot;
        if (gSystem.newKeys & PAD_KEY_UP) {
            selectedWcSlot = ov74_0222DAF8(appData, selectedWcSlot, -1);
        } else if (gSystem.newKeys & PAD_KEY_DOWN) {
            selectedWcSlot = ov74_0222DAF8(appData, selectedWcSlot, 1);
        } else if (gSystem.newKeys & PAD_BUTTON_B) {
            PlaySE(SEQ_SE_DP_SELECT);
            ov74_0222D7F0(appData, FADE_TYPE_BRIGHTNESS_OUT, WC_APP_STATE_EXIT, state);
        } else if (gSystem.newKeys & PAD_BUTTON_A) {
            PlaySE(SEQ_SE_DP_SELECT);
            *state = WC_APP_STATE_SHOW_WONDERCARD_ACTIONS;
        }

        if (appData->selectedWondercardSlot != selectedWcSlot) {
            PlaySE(SEQ_SE_DP_CARD2);
            appData->selectedWondercardSlot = selectedWcSlot;
            ov74_0222D824(appData, TRUE, WC_SCREEN_WONDERCARD_FRONT);
        }
        break;
    case WC_APP_STATE_SHOW_WONDERCARD_ACTIONS:
        appData->nextWindowBasetile = ov74_0222D9E0(appData, &appData->messageBox, MSG_BOX_TEXT_WONDER_CARD, 0x280);
        ov74_0222D9E0(appData, &appData->standardWindow, MSG_BOX_WC_ACTIONS_MENU, appData->nextWindowBasetile);
        *state = WC_APP_STATE_WAIT_FOR_MENU_CHOICE;
        break;
    case WC_APP_STATE_WAIT_FOR_MENU_CHOICE:
        ov74_0222DC60(man, state, NULL);
        if (gSystem.newKeys & PAD_BUTTON_B) {
            PlaySE(SEQ_SE_DP_SELECT);
            *state = WC_APP_STATE_CLOSE_WINDOWS;
        }
        break;
    case WC_APP_STATE_START_FLIP_WC_TO_BACK:
        PlaySE(SEQ_SE_DP_CARD2);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
        ov74_0222D104(&appData->messageBox, 0);
        ov74_0222D0EC(&appData->standardWindow, 0);
        ov74_0222DE8C(appData, WC_FLIP_STAGE_SHRINKING, 0x1000, 0x66);
        *state = WC_APP_STATE_WAIT_WC_FLIP_TO_BACK_HALFWAY;
        break;
    case WC_APP_STATE_WAIT_WC_FLIP_TO_BACK_HALFWAY:
        if (ov74_0222DF2C(appData)) {
            ov74_0222D824(appData, TRUE, WC_SCREEN_WONDERCARD_BACK);
            ov74_0222DE8C(appData, WC_FLIP_STAGE_GROWING, 0x708000, 0x384000);
            *state = WC_APP_STATE_SHOW_WONDERCARD_BACK;
        }
        break;
    case WC_APP_STATE_SHOW_WONDERCARD_BACK:
        ov74_0222DF2C(appData);
        if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            PlaySE(SEQ_SE_DP_CARD2);
            ov74_0222DE8C(appData, WC_FLIP_STAGE_SHRINKING, 0x1000, 0x66);
            *state = WC_APP_STATE_START_FLIP_WC_TO_FRONT;
        }
        break;
    case WC_APP_STATE_START_FLIP_WC_TO_FRONT:
        if (ov74_0222DF2C(appData)) {
            ov74_0222D824(appData, TRUE, WC_SCREEN_WONDERCARD_FRONT);
            ov74_0222DE8C(appData, WC_FLIP_STAGE_GROWING, 0x708000, 0x384000);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
            *state = WC_APP_STATE_WAIT_WC_FLIP_TO_FRONT_HALFWAY;
        }
        break;
    case WC_APP_STATE_WAIT_WC_FLIP_TO_FRONT_HALFWAY:
        if (ov74_0222DF2C(appData)) {
            DrawFrameAndWindow2(&appData->messageBox, FALSE, 0x13, 0xA);
            DrawFrameAndWindow1(&appData->standardWindow, FALSE, 10, 0xE);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
            ov74_0222DEF0(appData);
            *state = WC_APP_STATE_WAIT_FOR_MENU_CHOICE;
        }
        break;
    case WC_APP_STATE_CLOSE_WINDOWS:
        ov74_0222D448(appData);
        ov74_0222D104(&appData->messageBox, 0);
        ClearWindowTilemapAndCopyToVram(&appData->messageBox);
        RemoveWindow(&appData->messageBox);
        *state = WC_APP_STATE_SELECT_WONDERCARD;
        break;
    case WC_APP_STATE_WAIT_CONFIRM_DELETION:
        ov74_0222DC60(man, state, ov74_0222D414);
        break;
    case WC_APP_STATE_WAIT_CONFIRM_START_WIRELESS:
        ov74_0222DC60(man, state, ov74_0222D414);
        break;
    case WC_APP_STATE_ASK_START_WIRELESS_TO_SHARE_WC:
        ov74_0222D7A4(appData, &appData->standardWindow, MAKE_TEXT_COLOR(1, 2, 0));
        *state = WC_APP_STATE_WAIT_CONFIRM_START_WIRELESS;
        break;
    case WC_APP_STATE_START_TRANSITION_TO_WC_SHARE_SCREEN:
        ov74_0222D7F0(appData, FADE_TYPE_BRIGHTNESS_OUT, WC_APP_STATE_SHOW_WC_SHARE_SCREEN, state);
        break;
    case WC_APP_STATE_SHOW_WC_SHARE_SCREEN:
        ov74_0222D448(appData);
        ov74_0222D104(&appData->messageBox, 0);
        ClearWindowTilemapAndCopyToVram(&appData->messageBox);
        RemoveWindow(&appData->messageBox);
        ov74_0222D824(appData, FALSE, WC_SCREEN_WONDERCARD_SHARING);
        ov74_0222D248(appData->bgConfig);
        ov74_0222CFFC(appData);
        ov74_0222D098(appData);
        *state = WC_APP_STATE_PREPARE_FOR_SHARING;
        break;
    case WC_APP_STATE_PREPARE_FOR_SHARING:
        ov74_0222E7EC(appData);
        sub_0203A880();
        ov74_0222D7F0(appData, FADE_TYPE_BRIGHTNESS_IN, WC_APP_STATE_WAIT_FOR_PLAYERS, state);
        break;
    case WC_APP_STATE_WAIT_FOR_PLAYERS:
        connectedPlayersCount = 0;
        if (sub_020373B4(0)) {
            sub_020358B8(&appData->mysteryGiftAppData.eventData.header);
            connectedPlayersCount = ov74_0222E8B4(appData, &appData->windows[WC_SHARING_PLAYER_INFO]);
            if (connectedPlayersCount == 0 && appData->prevConnPlayersCount != 0) {
                FillWindowPixelBuffer(&appData->windows[WC_SHARING_PLAYER_INFO], 0);
                CopyWindowToVram(&appData->windows[WC_SHARING_PLAYER_INFO]);
                appData->prevConnPlayersCount = 0;
            }
            ov74_0222EA88(appData, &appData->windows[WC_SHARING_ENTRIES_COUNT], connectedPlayersCount);
            appData->prevConnPlayersCount = connectedPlayersCount;
        }
        ov74_0222E0D4(appData, connectedPlayersCount, state);
        break;
    case WC_APP_STATE_RETURN_AFTER_COMM_MAN_EXIT:
        ov74_0222D7F0(appData, FADE_TYPE_BRIGHTNESS_OUT, WC_APP_STATE_RETURN_TO_WC_ACTIONS, state);
        break;
    case WC_APP_STATE_RETURN_TO_WC_ACTIONS:
        ov74_0222D024(appData);
        ov74_0222D104(&appData->messageBox, 0);
        ov74_0222D0EC(&appData->standardWindow, 0);
        ov74_0222D824(appData, TRUE, WC_SCREEN_WONDERCARD_FRONT);
        LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, HEAP_ID_85);
        ov74_0222D7F0(appData, FADE_TYPE_BRIGHTNESS_IN, WC_APP_STATE_SHOW_WONDERCARD_ACTIONS, state);
        break;
    case WC_APP_STATE_WAIT_FOR_COMM_MAN_EXIT:
        if (sub_02037D78() == FALSE) {
            *state = appData->queuedState;
        }
        break;
    case WC_APP_STATE_DISTRIBUTION_UNDERWAY:
        if (--appData->shouldSendWc == 0) {
            ov74_02235ED0(&appData->mysteryGiftAppData.eventData, &appData->currentWonderCard, appData->heapID);
            ov74_02229D0C(&appData->currentWonderCard, sizeof(WonderCard));
            ov74_0222E060(appData);
            *state = WC_APP_STATE_START_COMM_SYNC;
        }
        break;
    case WC_APP_STATE_START_COMM_SYNC:
        if (ov74_0223615C() == 4) {
            sub_02037AC0(0x93);
            sub_020398D4(1, 1);
            *state = WC_APP_STATE_WAIT_FOR_COMM_SYNC;
        }
        break;
    case WC_APP_STATE_WAIT_FOR_COMM_SYNC:
        if (ov74_0222E898() == 0 || sub_02037B38(0x93) == TRUE) {
            sub_020398D4(0, 0);
            ov74_02236128();
            ov74_0222D9E0(appData, &appData->messageBox, MSG_BOX_DISTRIBUTION_SUCCESSFUL, 0x280);
            sub_0200F450(appData->waitDial);
            ov74_0222EB28(appData, state, WC_APP_STATE_DISTRIBUTION_SUCCESSFUL);
        }
        break;
    case WC_APP_STATE_DISTRIBUTION_SUCCESSFUL:
        if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            *state = WC_APP_STATE_RETURN_AFTER_COMM_MAN_EXIT;
        }
        break;
    case WC_APP_STATE_DISMISS_MESSAGE_BOX:
        if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            ov74_0222D104(&appData->messageBox, 0);
            ClearWindowTilemapAndCopyToVram(&appData->messageBox);
            RemoveWindow(&appData->messageBox);
            *state = WC_APP_STATE_SELECT_WONDERCARD;
        }
        break;
    case WC_APP_STATE_NO_MORE_WONDERCARDS:
        ov74_0222D7F0(appData, FADE_TYPE_BRIGHTNESS_OUT, WC_APP_STATE_EXIT, state);
        break;
    case WC_APP_STATE_WAIT_FOR_SCREEN_TRANSITION:
        if (IsPaletteFadeFinished()) {
            *state = appData->queuedState;
        }
        break;
    case WC_APP_STATE_EXIT:
        ov74_0222D024(appData);
        ov74_0222DEF0(appData);
        return TRUE;
    case WC_APP_STATE_UNUSED:
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
