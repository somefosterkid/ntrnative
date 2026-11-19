#include "global.h"
#include "overlay_96_app_021ed1d0.h"

s32 ov96_021ECDC4(Ov96PlayerSlot *arg0);

s32 ov96_021ECDC4(Ov96PlayerSlot *arg0) {
    if (arg0->counting == 0) {
        return 1;
    }
    return 0;
}

void ov96_021ECDD4(Ov96PlayerSlot *arg0, s32 arg1);

void ov96_021ECDD4(Ov96PlayerSlot *arg0, s32 arg1) {
    u32 v0;

    v0 = arg0->target + arg1;
    if (v0 > 0x3E7U) {
        v0 = 0x3E7;
    }
    arg0->target = v0;
    arg0->counting = 1;
}
