#include "global.h"

#include "sprite.h"
#include "bg_window.h"
#include "render_window.h"
#include "overlay_74_wonder_cards_app2_data.h"

void ov74_022359BC(void);

void ov74_0222EC60(WonderCardsApp2Data *appData);

void ov74_0222ECD4(Window *window, u8 skipTransfer);
void ov74_0222ECEC(Window *window, u8 skipTransfer);

void ov74_0222EC60(WonderCardsApp2Data *appData) {
    if (appData->shareScreenBtnSprites[0] != NULL) {
        Sprite_Delete(appData->shareScreenBtnSprites[0]);
    }
    if (appData->shareScreenBtnSprites[1] != NULL) {
        Sprite_Delete(appData->shareScreenBtnSprites[1]);
    }
    appData->shareScreenBtnSprites[1] = NULL;
    appData->shareScreenBtnSprites[0] = appData->shareScreenBtnSprites[1];
    if (appData->selectedWcSprites[0] != NULL) {
        Sprite_Delete(appData->selectedWcSprites[0]);
    }
    if (appData->selectedWcSprites[1] != NULL) {
        Sprite_Delete(appData->selectedWcSprites[1]);
    }
    if (appData->selectedWcSprites[2] != NULL) {
        Sprite_Delete(appData->selectedWcSprites[2]);
    }
    appData->selectedWcSprites[2] = NULL;
    appData->selectedWcSprites[1] = appData->selectedWcSprites[2];
    appData->selectedWcSprites[0] = appData->selectedWcSprites[1];
    ov74_022359BC();
    return;
}

void ov74_0222ECD4(Window *window, u8 skipTransfer) {
    if (WindowIsInUse(window) == 1) {
        ClearWindowPixelBuffer(window, skipTransfer);
    }
}

void ov74_0222ECEC(Window *window, u8 skipTransfer) {
    if (WindowIsInUse(window) == 1) {
        ClearFrameAndWindow2(window, skipTransfer);
    }
}
