#include "global.h"

#include "touchscreen.h"

extern u8 gSystem;

u32 ov56_021E5D44(s32 param_1);

u32 ov56_021E5D44(s32 param_1) {
    BOOL BVar1;

    if (*(u8 *)(param_1 + 10) == 1) {
        BVar1 = System_GetTouchHeld();
        if (BVar1 != 0) {
            return 0;
        }
        if (((s32 *)&gSystem)[0x11] != 0) {
            if (*(void (**)())(param_1 + 0x34) != NULL) {
                (**(void (**)(s32))(param_1 + 0x34))(param_1);
            }
            *(u8 *)(param_1 + 10) = 0;
            return 1;
        }
    } else {
        if (((s32 *)&gSystem)[0x11] != 0) {
            return 0;
        }
        BVar1 = System_GetTouchHeld();
        if (BVar1 != 0) {
            if (*(void (**)())(param_1 + 0x38) != NULL) {
                (**(void (**)(s32))(param_1 + 0x38))(param_1);
            }
            *(u8 *)(param_1 + 10) = 1;
            return 0;
        }
    }
    return 0;
}
