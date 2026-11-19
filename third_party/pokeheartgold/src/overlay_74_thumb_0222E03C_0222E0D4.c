#include "global.h"
#include "overlay_74_wonder_cards_app_data.h"
#include "constants/heap.h"
#include "heap.h"
#include "overlay_manager.h"
#include "sprite_resources_header.h"
#include "unk_02005D10.h"
#include "unk_020379A0.h"

enum WonderCardShareAction {
    WC_SHARE_ACTION_NONE = 0,
    WC_SHARE_ACTION_SEND,
    WC_SHARE_ACTION_EXIT,
    WC_SHARE_ACTION_PLAY_SOUND,
};
void ov74_0222EB28(WonderCardsAppData *appData, int *state, enum WonderCardsAppState nextState);
int ov74_0222D9E0(WonderCardsAppData *appData, Window *window, int windowTemplateIdx, int baseTile);
void ov74_0222E0D4(WonderCardsAppData *appData, int playerCount, int *state);
void ov74_0222E0D4(WonderCardsAppData *appData, int playerCount, int *state) {
    enum WonderCardShareBtn selectedButton = (enum WonderCardShareBtn)(appData->shareScreenSelectedBtn);

    if ((gSystem.newKeys & (0x0010)) && appData->shareScreenSelectedBtn != WC_SHARE_BTN_CANCEL) {
        appData->shareScreenSelectedBtn = WC_SHARE_BTN_CANCEL;
    }

    if ((gSystem.newKeys & (0x0020)) && appData->shareScreenSelectedBtn != WC_SHARE_BTN_SEND) {
        appData->shareScreenSelectedBtn = WC_SHARE_BTN_SEND;
    }

    if (selectedButton != appData->shareScreenSelectedBtn) {
        Sprite_SetAnimCtrlSeq(appData->shareScreenBtnSprites[WC_SHARE_BTN_SEND], appData->shareScreenSelectedBtn == WC_SHARE_BTN_SEND ? 1 : 0);
        Sprite_SetAnimCtrlSeq(appData->shareScreenBtnSprites[WC_SHARE_BTN_CANCEL], appData->shareScreenSelectedBtn == WC_SHARE_BTN_SEND ? 0 : 1);
    }

    enum WonderCardShareAction nextAction = WC_SHARE_ACTION_NONE;

    if (gSystem.newKeys & (0x0002)) {
        nextAction = WC_SHARE_ACTION_EXIT;
    } else if ((gSystem.newKeys & (0x0001)) && playerCount && appData->shareScreenSelectedBtn == WC_SHARE_BTN_SEND) {
        nextAction = WC_SHARE_ACTION_SEND;
    } else if ((gSystem.newKeys & (0x0001)) && appData->shareScreenSelectedBtn == WC_SHARE_BTN_CANCEL) {
        nextAction = WC_SHARE_ACTION_EXIT;
    } else if ((gSystem.newKeys & (0x0001)) && playerCount == 0) {
        nextAction = WC_SHARE_ACTION_PLAY_SOUND;
    }

    if (nextAction == WC_SHARE_ACTION_SEND) {
        PlaySE(1500);
        sub_02037AC0(0xAB);

        appData->shouldSendWc = 1;
        *state = WC_APP_STATE_DISTRIBUTION_UNDERWAY;
        ov74_0222D9E0(appData, &appData->messageBox, MSG_BOX_DISTRIBUTION_UNDER_WAY, 640);
        appData->waitDial = WaitingIcon_New(&appData->messageBox, ((1 + (3 * 3)) + (3 * 3)));
    }

    if (nextAction == WC_SHARE_ACTION_EXIT) {
        PlaySE(1500);
        ov74_0222EB28(appData, state, (enum WonderCardsAppState)20);
    }

    if (nextAction == WC_SHARE_ACTION_PLAY_SOUND) {
        PlaySE(1500);
    }
}
