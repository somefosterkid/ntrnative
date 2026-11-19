#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "touchscreen.h"

s32 ov59_0223A350(Ov59ApricornApp *, int);

extern TouchscreenHitbox ov59_0223C924;

s32 ov59_0223A418(Ov59ApricornApp *arg0, s32 *arg1);

s32 ov59_0223A418(Ov59ApricornApp *arg0, s32 *arg1) {
    int r;

    if ((r = TouchscreenHitbox_FindHitboxAtTouchNew(&ov59_0223C924)) == -1 || r == 7) {
        return 5;
    }
    *arg1 = 1;
    arg0->menuInputState = MENU_INPUT_STATE_TOUCH;
    arg0->unk_4C = 0;
    arg0->unk_4B = arg0->unk_4C;
    arg0->unk_4A = 0;
    if (r == 8) {
        arg0->unk_49 = 2;
    } else {
        arg0->unk_49 = r % 2;
        arg0->unk_4A = r / 2;
    }
    return ov59_0223A350(arg0, r);
}
