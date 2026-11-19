#include "global.h"

#include "list_menu.h"
#include "overlay_74_wonder_cards_app2_data.h"
#include "overlay_manager.h"
#include "unk_02005D10.h"

extern WonderCardsApp2StateFunc ov74_0223D0C0;

void ov74_0222F404(OverlayManager *man, int *state, WonderCardsApp2StateFunc onCancel);

void ov74_0222F404(OverlayManager *man, int *state, WonderCardsApp2StateFunc onCancel) {
    int nextState;
    WonderCardsApp2Data *appData = OverlayManager_GetData(man);
    u32 input = ListMenu_ProcessInput(appData->listMenu);

    switch (input) {
    case LIST_NOTHING_CHOSEN:
        break;
    case LIST_CANCEL:
        PlaySE(SEQ_SE_DP_SELECT);
        if (onCancel) {
            nextState = onCancel(man);
            if (nextState != -1) {
                *state = nextState;
            }
        }
        break;
    default:
        PlaySE(SEQ_SE_DP_SELECT);
        if (input) {
            if (input < 31) {
                *state = input;
            } else {
                ov74_0223D0C0 = (WonderCardsApp2StateFunc)input;
                nextState = ov74_0223D0C0(man);
                if (nextState != -1) {
                    *state = nextState;
                }
            }
        }
        break;
    }
}
