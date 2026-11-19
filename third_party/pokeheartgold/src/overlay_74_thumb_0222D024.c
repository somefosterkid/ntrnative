#include "global.h"
#include "overlay_74_wonder_cards_app_data.h"
#include "constants/heap.h"
#include "heap.h"
#include "overlay_manager.h"
#include "sprite_resources_header.h"

void ov74_022359BC(void);
Sprite *ov74_02235930(enum GFScreen screen, Sprite *sprite, int x, int y, int animationIdx);
void ov74_0222D024(WonderCardsAppData *appData);
void ov74_0222D098(WonderCardsAppData *appData);
void ov74_0222D0EC(Window *window, u8 skipTransfer);
void ov74_0222D104(Window *window, u8 skipTransfer);
void ov74_0222D024(WonderCardsAppData *appData) {
    if (appData->shareScreenBtnSprites[0]) {
        Sprite_Delete(appData->shareScreenBtnSprites[0]);
    }

    if (appData->shareScreenBtnSprites[1]) {
        Sprite_Delete(appData->shareScreenBtnSprites[1]);
    }

    appData->shareScreenBtnSprites[0] = appData->shareScreenBtnSprites[1] = 0L;

    if (appData->selectedWcSprites[0]) {
        Sprite_Delete(appData->selectedWcSprites[0]);
    }

    if (appData->selectedWcSprites[1]) {
        Sprite_Delete(appData->selectedWcSprites[1]);
    }

    if (appData->selectedWcSprites[2]) {
        Sprite_Delete(appData->selectedWcSprites[2]);
    }

    appData->selectedWcSprites[0] = appData->selectedWcSprites[1] = appData->selectedWcSprites[2] = 0L;
    ov74_022359BC();
}
void ov74_0222D098(WonderCardsAppData *appData) {
    appData->shareScreenSelectedBtn = WC_SHARE_BTN_SEND;
    appData->shareScreenBtnSprites[WC_SHARE_BTN_SEND] = ov74_02235930(SCREEN_MAIN, appData->shareScreenBtnSprites[WC_SHARE_BTN_SEND], 72, 168, 1);
    Sprite_SetPriority(appData->shareScreenBtnSprites[WC_SHARE_BTN_SEND], 2);

    appData->shareScreenBtnSprites[WC_SHARE_BTN_CANCEL] = ov74_02235930(SCREEN_MAIN, appData->shareScreenBtnSprites[WC_SHARE_BTN_CANCEL], 184, 168, 0);
    Sprite_SetPriority(appData->shareScreenBtnSprites[WC_SHARE_BTN_CANCEL], 2);
}
void ov74_0222D0EC(Window *window, u8 skipTransfer) {
    if (WindowIsInUse(window) == 1) {
        ClearWindowPixelBuffer(window, skipTransfer);
    }
}
void ov74_0222D104(Window *window, u8 skipTransfer) {
    if (WindowIsInUse(window) == 1) {
        ClearFrameAndWindow2(window, skipTransfer);
    }
}
