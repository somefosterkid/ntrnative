#include "global.h"
#include "overlay_59_apricorn_app.h"

s32 ov59_02238FF4(Ov59ApricornApp *, s32);

s32 ov59_02239F18();

s32 ov59_022380EC(Ov59ApricornApp *arg0);

s32 ov59_022380EC(Ov59ApricornApp *arg0) {
    s32 var_r0;
    u16 temp_r1;

    temp_r1 = arg0->state;
    switch (temp_r1) {
    case 0:
        ov59_02238FF4(arg0, 0);
        arg0->state = arg0->state + 1;
        break;
    case 1:
        var_r0 = ov59_02239F18();
        if (var_r0 != 4) {
            arg0->state = 0;
        }
        return var_r0;
    }

    return 4;
}
