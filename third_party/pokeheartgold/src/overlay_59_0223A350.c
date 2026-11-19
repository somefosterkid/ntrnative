#include "global.h"
#include "overlay_59_apricorn_app.h"
#include "unk_02005D10.h"

s32 ov59_022387D0(Ov59ApricornApp *, s32);

s32 ov59_022388C4(Ov59ApricornApp *);

s32 ov59_0223A350(Ov59ApricornApp *arg0, u32 arg1);

s32 ov59_0223A350(Ov59ApricornApp *arg0, u32 arg1) {
    if (arg1 <= 6U) {
        PlaySE(0x5DCU);
        ov59_022388C4(arg0);
        if (arg0->apricornCounts[arg1] == 0) {
            return 5;
        }
        arg0->selected = (u8) arg1;
        arg0->quantity = 1;
        arg0->maxQuantity = (u8) arg0->apricornCounts[arg0->selected];
        return 6;
    }
    if (arg1 == 8) {
        PlaySE(0x5DCU);
        ov59_022387D0(arg0, 1);
        return 1;
    }
    return 5;
}
