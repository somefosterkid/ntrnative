#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "overlay_74_wonder_cards_app2_data.h"

void ov74_0222ED04(WonderCardsApp2Data *appData, const WonderCardsApp2MenuOption *options, int count, Window *window, u32 itemsAbove);

extern const WonderCardsApp2MenuOption ov74_0223C6D0[2];

BOOL ov74_0222EF68(WonderCardsApp2Data *appData, Window *window);

BOOL ov74_0222EF68(WonderCardsApp2Data *appData, Window *window) {
    WonderCardsApp2MenuOption options[2];
    WonderCardsApp2WindowTemplate *windowTemplate;

    options[0] = ov74_0223C6D0[0];
    options[1] = ov74_0223C6D0[1];
    windowTemplate = appData->messageBoxTemplate;
    ClearWindowTilemapAndCopyToVram(window);
    RemoveWindow(window);
    AddWindowParameterized(appData->bgConfig, window, GF_BG_LYR_MAIN_2, windowTemplate->tilemapLeft, windowTemplate->tilemapTop, windowTemplate->width, windowTemplate->height, 15, appData->messageBoxBaseTile);
    ov74_0222ED04(appData, options, 2, window, 0);
    return TRUE;
}
