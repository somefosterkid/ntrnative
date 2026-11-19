#include "global.h"

#include "overlay_73_unk_struct_020961_e8_t.h"
#include "system.h"
#include "touchscreen.h"

u32 ov73_021E64B0(UnkStruct_020961E8 *param_1);

u32 ov73_021E64B0(UnkStruct_020961E8 *param_1) {
    BOOL BVar1;

    if (param_1->unk_314 == MENU_INPUT_STATE_TOUCH) {
        BVar1 = System_GetTouchHeld();
        if (BVar1 != 0) {
            return 0;
        }
        if (gSystem.heldKeys != 0) {
            param_1->unk_314 = MENU_INPUT_STATE_BUTTONS;
            return 1;
        }
    } else {
        if (gSystem.heldKeys != 0) {
            return 0;
        }
        BVar1 = System_GetTouchHeld();
        if (BVar1 != 0) {
            param_1->unk_314 = MENU_INPUT_STATE_TOUCH;
            return 0;
        }
    }
    return 0;
}
