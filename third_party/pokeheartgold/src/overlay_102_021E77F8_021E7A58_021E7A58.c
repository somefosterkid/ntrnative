#include "global.h"

#include "touchscreen.h"

extern u8 gSystem;

u32 ov102_021E7A58(void *param_1);

u32 ov102_021E7A58(void *param_1) {
    BOOL BVar1;

    if (*(s32 *)(param_1 + 0x74) == 1) {
        BVar1 = System_GetTouchHeld();
        if (BVar1 != 0) {
            return 0;
        }
        if (((s32 *)&gSystem)[0x11] != 0) {
            *(u32 *)(param_1 + 0x38) = 4;
            *(u32 *)(param_1 + 0x74) = 0;
            return 0;
        }
    } else {
        if (((s32 *)&gSystem)[0x11] != 0) {
            return 0;
        }
        BVar1 = System_GetTouchHeld();
        if (BVar1 != 0) {
            *(u32 *)(param_1 + 0x74) = 1;
            return 0;
        }
    }
    return 0;
}
