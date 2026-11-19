#include "global.h"

#include "overlay_72_wfc_connect.h"

void ov72_022386F4(Sprite *, s32 arg1);

s32 ov72_0223A520(NintendoWfcConnect *arg0, s8 arg1, s8 arg2);

s32 ov72_0223A520(NintendoWfcConnect *arg0, s8 arg1, s8 arg2) {
    arg0->numberInput.unk_0 = arg1;
    arg0->numberInput.maxValue = arg2;
    arg0->numberInput.value = 0;
    arg0->numberInput.cursor = 0;
    if (arg2 != 0x64) {
        arg0->numberInput.digitPos = 1;
        arg0->twoDigits = 1;
    } else {
        arg0->numberInput.digitPos = 0;
        arg0->twoDigits = 0;
    }
    arg0->numberInput.state = 0;
    arg0->numberInput.digits[0] = -1;
    arg0->numberInput.digits[1] = -1;
    arg0->numberInput.digits[2] = -1;
    arg0->padColumn[0] = 0;
    arg0->padColumn[1] = 1;
    Sprite *ptr = arg0->sprites[3];
    if (ptr != NULL) {
        ov72_022386F4(ptr, 0);
    }
    return 1;
}
