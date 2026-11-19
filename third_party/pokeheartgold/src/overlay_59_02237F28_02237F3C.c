#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "unk_020210A0.h"

void ov59_02238674(Ov59ApricornApp *arg0);

s32 ov59_022386D0();

s32 ov59_02237F3C(Ov59ApricornApp *arg0);

s32 ov59_02237F3C(Ov59ApricornApp *arg0) {
    u16 temp_r1;

    temp_r1 = arg0->state;
    switch (temp_r1) {
    case 0:
        ov59_02238674(arg0);
        arg0->state = (u16)(arg0->state + 1);
        break;
    case 1:
        ov59_022386D0();
        EnableTouchPad();
        InitializeTouchPad(1);
        arg0->state = 0U;
        return 1;
    }
    return 0;
}
