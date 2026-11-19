#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "touchscreen.h"

s32 ov59_0223A194(Ov59ApricornApp *, int);

extern TouchscreenHitbox ov59_0223C924;

s32 ov59_0223A2D0(Ov59ApricornApp *arg0, s32 *arg1);

s32 ov59_0223A2D0(Ov59ApricornApp *arg0, s32 *arg1) {
    int r;

    r = TouchscreenHitbox_FindHitboxAtTouchNew(&ov59_0223C924);
    if (r == TOUCH_MENU_NO_INPUT) {
        return 4;
    }
    *arg1 = 1;
    arg0->menuInputState = MENU_INPUT_STATE_TOUCH;
    arg0->unk_4C = 0;
    arg0->unk_4B = arg0->unk_4C;
    arg0->unk_4A = 0;
    switch (r) {
    case 7:
        arg0->unk_49 = 2;
        break;
    case 8:
        arg0->unk_49 = 3;
        break;
    default:
        arg0->unk_49 = r % 2;
        arg0->unk_4A = r / 2;
        break;
    }
    return ov59_0223A194(arg0, r);
}
