#include "global.h"

#include "system.h"
#include "overlay_34.h"

void ov34_0225E58C(Ov34Work *work) {
    work->touchTrigger = 0;
    if (gSystem.touchNew != 0) {
        work->touchTrigger = 1;
        return;
    }
    if (gSystem.touchHeld != 0) {
        work->repeatCounter = (s8)(work->repeatCounter - 1);
        if ((s32) work->repeatCounter < 0) {
            work->touchTrigger = 1;
            work->repeatCounter = (s8) work->repeatInterval;
        }
    } else {
        work->repeatCounter = (s8) work->repeatStart;
    }
}

u8 ov34_0225E5D4(Ov34Work *work) {
    return work->touchTrigger;
}

void ov34_0225E5DC(Ov34Work *work, s32 arg1) {
    work->scrollbarHeld = arg1;
}

s32 ov34_0225E5E4(Ov34Work *work) {
    return work->scrollbarHeld;
}
