#include "global.h"

#include "overlay_34.h"

void ov34_0225E560(Ov34Work *work) {
    work->needsRedraw = 1;
}

void ov34_0225E56C(Ov34Work *work) {
    work->touchTrigger = 0;
    work->repeatStart = 8;
    work->repeatInterval = 4;
    work->repeatCounter = work->repeatStart;
}
