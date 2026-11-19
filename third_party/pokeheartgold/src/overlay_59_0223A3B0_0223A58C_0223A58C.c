#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "system.h"
#include "unk_02005D10.h"

s32 ov59_0223A48C(Ov59ApricornApp *, s32);

s32 ov59_0223A58C(Ov59ApricornApp *arg0);

s32 ov59_0223A58C(Ov59ApricornApp *arg0) {
    s32 temp_r3;

    if (0xCF3 & gSystem.newKeys) {
        arg0->menuInputState = MENU_INPUT_STATE_BUTTONS;
    }
    if (gSystem.newKeys & 2) {
        return ov59_0223A48C(arg0, 1);
    }
    if (1 & gSystem.newKeys) {
        return ov59_0223A48C(arg0, 0);
    }
    if (0x40 & gSystem.newKeys) {
        return ov59_0223A48C(arg0, 2);
    }
    temp_r3 = gSystem.newKeys & 0x80;
    if (temp_r3 != 0) {
        return ov59_0223A48C(arg0, 3);
    }
    if (0x10 & gSystem.newKeys) {
        return ov59_0223A48C(arg0, 4);
    }
    if (0x20 & gSystem.newKeys) {
        return ov59_0223A48C(arg0, 5);
    }
    if (temp_r3 != 0) {
        PlaySE(0x5DCU);
        return 6;
    }
    return 6;
}
