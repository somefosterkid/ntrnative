#include "global.h"

#include "overlay_96_ov96hurdle_work.h"

void ov96_02206F1C(Ov96HurdleWork *arg0, s32 arg1, s32 arg2, Ov96HurdlePoint **arg3, u8 *arg4);

void ov96_02206F1C(Ov96HurdleWork *arg0, s32 arg1, s32 arg2, Ov96HurdlePoint **arg3, u8 *arg4) {
    s32 i;
    s32 x;
    s32 y;
    s32 sx;
    s32 sy;

    for (i = 0; i < 20; i++) {
        arg3[i] = NULL;
    }
    sx = arg1 / 4096;
    sy = arg2 / 4096;
    *arg4 = 0;
    for (i = 0; i < 20; i++) {
        if (arg0->cells[i].active == 0) {
            break;
        }
        x = (arg0->cells[i].x - sx) + 0x80;
        y = (arg0->cells[i].y - sy) + 0x60;
        if (x >= -0x20 && x <= 0x120 && y >= -0x20 && y <= 0xE0) {
            arg3[(*arg4)++] = &arg0->points_61C[i];
        }
    }
}
