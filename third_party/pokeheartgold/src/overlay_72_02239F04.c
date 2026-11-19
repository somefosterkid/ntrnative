#include "global.h"
#include "overlay_72_wfc_connect.h"
#include "overlay_72_wfc_state.h"

s32 ov72_0223AF48(NintendoWfcYesNoMenu *);

u32 ov72_02239F04(NintendoWfcConnect *param_1);

s32 ov72_02238680(void *, s32, s32);
s32 ov72_0223A350(void *, void *, s32, s32, s32);
s32 ov72_02239F30(NintendoWfcConnect *arg0);

u32 ov72_02239F04(NintendoWfcConnect *param_1) {
    s32 iVar1;

    iVar1 = ov72_0223AF48(&param_1->yesNoMenu);
    if (iVar1 != 0) {
        if (iVar1 == 2) {
            param_1->state = NINTENDO_WFC_STATE_ASK_CHALLENGE_ROOM;
        } else {
            param_1->state = NINTENDO_WFC_STATE_PRINT_DISCONNECTING;
            param_1->args->unk20 = 1;
        }
    }
    return 3;
}

s32 ov72_02239F30(NintendoWfcConnect *arg0) {
    ov72_0223A350(arg0, arg0->msgDataTower, 9, 1, 0xF0F);
    ov72_02238680(arg0, 0x30, 0x18);
    return 3;
}
