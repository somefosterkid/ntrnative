#include "global.h"
#include "overlay_59_apricorn_app.h"
#include "unk_02005D10.h"

s32 ov59_022387D0(Ov59ApricornApp *, s32);

s32 ov59_022388C4(Ov59ApricornApp *);

s32 ov59_0223A07C(Ov59ApricornApp *arg0, u32 arg1);

s32 ov59_0223A07C(Ov59ApricornApp *arg0, u32 arg1) {
    if (arg1 <= 6U) {
        PlaySE(0x5DCU);
        ov59_022388C4(arg0);
        arg0->selected = (s8) arg1;
        return 3;
    }
    if (arg1 == 8) {
        PlaySE(0x5DCU);
        ov59_022387D0(arg0, 1);
        return 1;
    }
    return 3;
}
