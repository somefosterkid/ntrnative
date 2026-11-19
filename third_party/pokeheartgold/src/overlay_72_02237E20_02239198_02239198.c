#include <dwc.h>

#include "global.h"

#include "overlay_72_wfc_state.h"
#include "overlay_72_wfc_connect.h"
struct NintendoWfcConnect;

s32 ov72_022387D0(struct NintendoWfcConnect *, s32, s32);

s32 ov72_0223AF48(NintendoWfcYesNoMenu *);

s32 ov72_02239198(NintendoWfcConnect *arg0);

s32 ov72_02239198(NintendoWfcConnect *arg0) {
    s32 temp_r0;
    enum NintendoWfcState var_r0;

    temp_r0 = (s32)(ov72_0223AF48(&arg0->yesNoMenu));
    if (temp_r0 != 0) {
        if (temp_r0 == 2) {
            if (DWC_CheckInet() == 0) {
                arg0->state = NINTENDO_WFC_STATE_ASK_CONNECT;
            } else {
                arg0->state = NINTENDO_WFC_STATE_PRINT_DISCONNECTING;
            }
        } else {
            if (DWC_CheckInet() == 0) {
                ov72_022387D0(arg0, 0, 0);
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
