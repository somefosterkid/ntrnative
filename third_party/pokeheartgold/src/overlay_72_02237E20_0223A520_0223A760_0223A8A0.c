#include "global.h"

#include "overlay_72_wfc_connect.h"

s32 ov72_0223A8A0(NintendoWfcNumberInput *arg0);

s32 ov72_0223A8A0(NintendoWfcNumberInput *arg0) {
    if (arg0->maxValue != 0x64) {
        if (((s32)arg0->digits[1] >= 0) && ((s32)arg0->digits[2] >= 0)) {
            return 1;
        }
    } else {
        if (((s32)arg0->digits[0] >= 0) && ((s32)arg0->digits[1] >= 0) && ((s32)arg0->digits[2] >= 0)) {
            return 1;
        }
    }
    return 0;
}
