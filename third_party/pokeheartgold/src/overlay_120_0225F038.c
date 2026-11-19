#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_120.h"
#include "sys_task.h"

void ov120_0225F038(Ov120Tile *tile) {
    Heap_Free(tile);
}

void ov120_0225F040(Ov120Tile *tile, s32 x, s32 y, s32 numSteps, Window *window, s32 widthStart, s32 heightStart, s32 widthEnd, s32 heightEnd, u8 color) {
    if (tile->active != 0) {
        GF_AssertFail();
    }
    ov01_021EFE34(&tile->width, widthStart, widthEnd, numSteps);
    ov01_021EFE34(&tile->height, heightStart, heightEnd, numSteps);
    tile->window = window;
    tile->x = x;
    tile->y = y;
    tile->color = (u8)color;
    tile->active = 1U;
}
