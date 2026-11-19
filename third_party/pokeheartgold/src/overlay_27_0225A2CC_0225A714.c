#include "global.h"

#include "error_handling.h"
#include "field_system.h"
#include "menu_input_state.h"
#include "overlay_27.h"
#include "overlay_27_ov27task.h"

s32 ov27_0225A8E8(Ov27Task *, s32);

s32 ov27_0225B398(Ov27Task *, s32);

s32 ov27_0225A714(Ov27Task *arg0) {
    u16 *v2;
    u32 v0;
    FieldSystem *v1;

    if (arg0->unk_0C == NULL) {
        GF_AssertFail();
    }
    v1 = arg0->fieldSystem;
    v2 = arg0->unk_0C;
    v0 = v1->unkD2_0;
    if (v0 == 4) {
        if (MenuInputStateMgr_GetState(&v1->menuInputState) == MENU_INPUT_STATE_BUTTONS) {
            ov27_0225A8E8(arg0, 1);
        } else {
            v1->unkD2_0 = 0;
        }
    } else if (v0 == 3) {
        if (MenuInputStateMgr_GetState(&v1->menuInputState) == MENU_INPUT_STATE_TOUCH) {
            *v2 = 1;
            v1->unkD2_0 = 0;
            v1->unkD2_7 = TRUE;
        } else {
            ov27_0225B398(arg0, arg0->unk_14);
            v1->unkD2_0 = 1;
        }
    }
    return 1;
}

s32 ov27_0225A66C(Ov27Task *);

u32 ov27_0225AAD4(Ov27Task *);

u32 ov27_0225A7B0(Ov27Task *arg0) {
    u32 v0;

    if (sub_0203DF8C(arg0->fieldSystem) == 1) {
        ov27_0225A66C(arg0);
    }
    v0 = arg0->unk_51C_1;
    if (v0 == 0) {
        v0 = ov27_0225AAD4(arg0);
    }
    return v0;
}

s32 ov27_0225A690(Ov27Task *, s32);

void ov27_0225A7DC(Ov27Task *arg0) {
    if ((s32) arg0->unk_14 >= 7) {
        GF_AssertFail();
    }
    ov27_0225B398(arg0, arg0->unk_14);
    ov27_0225A690(arg0, 0);
}
