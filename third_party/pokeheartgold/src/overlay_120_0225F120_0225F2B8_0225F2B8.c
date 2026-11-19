#include "global.h"

#include "error_handling.h"
#include "overlay_120.h"

void ov120_0225F2B8(Ov120Tiles *tiles, u8 interval, u8 steps, Window *window, u8 color, u32 reverse) {
    if (window == NULL) {
        GF_AssertFail();
    }
    tiles->window = window;
    tiles->color = color;
    tiles->interval = interval;
    tiles->steps = steps;
    tiles->count = 0;
    tiles->done = 0;
    tiles->timer = interval;
    tiles->active = 1;
    tiles->reverse = reverse;
}
