#include "global.h"
#include "overlay_59_apricorn_app.h"
#include "unk_02005D10.h"
#include "system.h"

s32 ov59_022388C4(Ov59ApricornApp *);

u8 ov59_02239EA8(Ov59ApricornApp *);

s32 ov59_0223A350(Ov59ApricornApp *, s32);

s32 ov59_0223A678(Ov59ApricornApp *);

s32 ov59_0223A3B0(Ov59ApricornApp *arg0);

s32 ov59_0223A3B0(Ov59ApricornApp *arg0) {
    if (0xCF3 & gSystem.newKeys) {
        arg0->menuInputState = MENU_INPUT_STATE_BUTTONS;
    }
    if (2 & gSystem.newKeys) {
        return ov59_0223A350(arg0, 8);
    }
    if (1 & gSystem.newKeys) {
        return ov59_0223A350(arg0, ov59_02239EA8(arg0));
    }
    if (ov59_0223A678(arg0) != 0) {
        PlaySE(0x5DCU);
        ov59_022388C4(arg0);
        return 5;
    }
    return 5;
}
