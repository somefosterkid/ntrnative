#include "global.h"
#include "overlay_72_wfc_connect.h"

s32 ov72_0223AE20(NintendoWfcConnect *, u8, s32, s32);

s32 ov72_0223AE6C(NintendoWfcConnect *arg0, s32 arg1, s32 arg2, s32 arg3);

s32 ov72_0223AE6C(NintendoWfcConnect *arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8 temp_r1;
    NintendoWfcButtonFlash *temp_r4;

    temp_r4 = &arg0->buttonFlash;
    temp_r1 = temp_r4->state;
    switch (temp_r1) {
    case 0:
        ov72_0223AE20(arg0, (u8)temp_r4->pressedPltt, arg2, arg3);
        temp_r4->state = (u8)(temp_r4->state + 1);
        break;
    case 1:
        temp_r4->timer = (u8)(temp_r4->timer + 1);
        if (temp_r4->timer == 4) {
            ov72_0223AE20(arg0, (u8)temp_r4->normalPltt, arg2, arg3);
            temp_r4->timer = 0U;
            temp_r4->state = (u8)(temp_r4->state + 1);
        }
        break;
    case 2:
        temp_r4->timer = (u8)(temp_r4->timer + 1);
        if (temp_r4->timer == 2) {
            return 0;
        }
        break;
    }
    return 1;
}
