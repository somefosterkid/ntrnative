#include "global.h"

#include "heap.h"
#include "overlay_120.h"
#include "sys_task.h"

void ov120_0225F118(Ov120Wipe *wipe) {
    Heap_Free(wipe);
}

void ov120_0225F120(Ov120Wipe *wipe, s8 interval, Window *window, s8 color) {
    wipe->window = window;
    wipe->color = color;
    wipe->interval = interval;
    wipe->count = 0;
    wipe->timer = 0;
    wipe->active = 1;
    ov120_0225F1E8(&wipe->shuffle, 0x30);
}
