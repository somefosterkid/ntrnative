#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "unk_020210A0.h"

s32 ov59_02238624(void);

s32 ov59_022386A8(Ov59ApricornApp *);

s32 ov59_02237F74(Ov59ApricornApp *arg0);

s32 ov59_02237F74(Ov59ApricornApp *arg0) {
    switch (arg0->state) {
    case 0:
        DisableTouchPad();
        ov59_022386A8(arg0);
        arg0->state = (u16)((arg0->state) + 1);
        break;
    case 1:
    default:
        ov59_02238624();
        arg0->state = 0U;
        return 1;
    }
    return 0;
}
