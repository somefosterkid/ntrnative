#include <dwc.h>

#include "global.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

s32 ov73_021E83D4(void *);

s32 ov73_021E844C(void *, s32, s32);

s32 ov73_021E8CBC(WifiBattleTowerAppState *arg0);

s32 ov73_021E8CBC(WifiBattleTowerAppState *arg0) {
    s32 temp_r0;
    enum NintendoWfcState var_r0;

    temp_r0 = ov73_021E83D4(arg0->yesNoPrompt);
    if (temp_r0 != 0) {
        if (temp_r0 == 2) {
            if (DWC_CheckInet() == 0) {
                arg0->state = NINTENDO_WFC_STATE_ASK_CONNECT;
            } else {
                arg0->state = NINTENDO_WFC_STATE_PRINT_DISCONNECTING;
            }
        } else {
            if (DWC_CheckInet() == 0) {
                ov73_021E844C(arg0, 0, 0);
                var_r0 = NINTENDO_WFC_STATE_START_EXIT;
            } else {
                var_r0 = NINTENDO_WFC_STATE_PRINT_DISCONNECTING;
            }
            arg0->state = var_r0;
            arg0->args->unk20 = 1;
        }
    }
    return 3;
}
