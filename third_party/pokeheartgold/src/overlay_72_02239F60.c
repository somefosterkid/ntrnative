#include "global.h"
#include "overlay_72_wfc_connect.h"
#include "overlay_72_wfc_state.h"

s32 ov72_0223AF48(NintendoWfcYesNoMenu *);

u32 ov72_02239F60(NintendoWfcConnect *param_1);

s32 ov72_02238680(void *, s32, s32);
s32 ov72_0223A350(void *, void *, s32, s32, s32);
s32 ov72_02239F8C(NintendoWfcConnect *arg0);

u32 ov72_02239F60(NintendoWfcConnect *param_1) {
    s32 iVar1;

    iVar1 = ov72_0223AF48(&param_1->yesNoMenu);
    if (iVar1 != 0) {
        if (iVar1 == 2) {
            param_1->state = NINTENDO_WFC_STATE_ASK_LEADERS_RANK;
        } else {
            param_1->state = NINTENDO_WFC_STATE_PRINT_DISCONNECTING;
            param_1->args->unk20 = 1;
        }
    }
    return 3;
}

s32 ov72_02239F8C(NintendoWfcConnect *arg0) {
    ov72_0223A350(arg0, arg0->msgDataError, 0x1A, 1, 0xF0F);
    ov72_02238680(arg0, 0x2F, 0x2A);
    return 3;
}
