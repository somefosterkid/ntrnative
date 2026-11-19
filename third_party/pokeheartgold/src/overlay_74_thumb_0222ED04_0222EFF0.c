#include "global.h"

#include "constants/heap.h"

#include "overlay_74_wonder_cards_app2_data.h"
#include "screen_fade.h"

void ov74_0222EFF0(WonderCardsApp2Data *appData, enum FadeType fadeType, int nextState, int *state);

void ov74_0222EFF0(WonderCardsApp2Data *appData, enum FadeType fadeType, int nextState, int *state) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, fadeType, fadeType, 0U, 6, 1, HEAP_ID_85);
    if (state != NULL) {
        *state = 0x1C;
    }
    appData->queuedState = nextState;
}
