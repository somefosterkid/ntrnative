#include "global.h"
#include "overlay_72_wfc_state.h"

#include "unk_02037C94.h"
#include "overlay_72_wfc_connect.h"

s32 ov72_02238680(void *, s32, s32);

s32 ov72_022387D0(NintendoWfcConnect *, s32, s32);

s32 ov72_0223A350(void *, MsgData *, s32, s32, s32);

s32 ov72_0223A420(NintendoWfcConnect *);

s32 ov72_0223AF48(NintendoWfcYesNoMenu *);

s32 ov72_02239108(NintendoWfcConnect *arg0);

s32 ov72_02239108(NintendoWfcConnect *arg0) {
    s32 temp_r0;

    temp_r0 = (s32)(ov72_0223AF48(&arg0->yesNoMenu));
    if (temp_r0 != 0) {
        if (temp_r0 == 2) {
            sub_0203946C();
            ov72_022387D0(arg0, 0, 0);
            arg0->state = NINTENDO_WFC_STATE_START_EXIT;
        } else {
            ov72_0223A350(arg0, arg0->msgDataWfc, 1, 1, 0xF0F);
            ov72_02238680(arg0, 0x2F, 2);
            ov72_0223A420(arg0);
        }
    }
    return 3;
}
