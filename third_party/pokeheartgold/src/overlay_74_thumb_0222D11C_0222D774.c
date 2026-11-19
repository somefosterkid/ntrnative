#include "global.h"

#include "constants/heap.h"

#include "message_format.h"
#include "overlay_74_wonder_cards_app_data.h"
#include "screen_fade.h"

void ov74_0222D11C(WonderCardsAppData *appData, StateTransitionMenuOptionTemplate *entries, int numEntries, Window *window, int startCursorPos);
void ov74_0222D1D4(Window *window, int textEntryID);

extern StateTransitionMenuOptionTemplate ov74_0223C2D0[2];
extern StateTransitionMenuOptionTemplate ov74_0223C2E0[2];
extern StateTransitionMenuOptionTemplate ov74_0223C2F0[2];

BOOL ov74_0222D774(WonderCardsAppData *appData, Window *window, u32 color);
BOOL ov74_0222D78C(WonderCardsAppData *appData, Window *window, u32 color);
BOOL ov74_0222D7A4(WonderCardsAppData *appData, Window *window, u32 color);
BOOL ov74_0222D7D0(WonderCardsAppData *appData, Window *window, u32 color);
void ov74_0222D7F0(WonderCardsAppData *appData, enum FadeType fadeType, enum WonderCardsAppState nextState, int *state);

BOOL ov74_0222D774(WonderCardsAppData *appData, Window *window, u32 color) {
    ov74_0222D11C(appData, ov74_0223C2D0, 2, window, 1);
    return TRUE;
}

BOOL ov74_0222D78C(WonderCardsAppData *appData, Window *window, u32 color) {
    ov74_0222D11C(appData, ov74_0223C2F0, 2, window, 0);
    return TRUE;
}

BOOL ov74_0222D7A4(WonderCardsAppData *appData, Window *window, u32 color) {
    ov74_0222D1D4(&appData->messageBox, 2);
    ov74_0222D11C(appData, ov74_0223C2E0, 2, window, 0);
    return TRUE;
}

BOOL ov74_0222D7D0(WonderCardsAppData *appData, Window *window, u32 color) {
    BufferIntegerAsString(appData->strTemplate, 0, 0, 1, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    return TRUE;
}

void ov74_0222D7F0(WonderCardsAppData *appData, enum FadeType fadeType, enum WonderCardsAppState nextState, int *state) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, fadeType, fadeType, 0, 6, 1, HEAP_ID_85);
    if (state != NULL) {
        *state = WC_APP_STATE_WAIT_FOR_SCREEN_TRANSITION;
    }
    appData->queuedState = nextState;
}
