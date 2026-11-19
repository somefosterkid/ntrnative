#include "global.h"

#include "bg_window.h"
#include "overlay_96_ov96_digit_work.h"

typedef struct Ov96DigitPos {
    u8 y[3];
    u8 x[3];
} Ov96DigitPos;

void ov96_02219A5C(Ov96DigitWork *work, s32 value);

void ov96_02219A5C(Ov96DigitWork *work, s32 value) {
    u8 x[3];
    u8 y[3];
    u8 digit;
    u8 i;

    digit = value / 100;
    x[0] = digit % 5 * 4;
    y[0] = digit / 5 * 8;
    digit = (value - digit * 100) / 10;
    x[1] = digit % 5 * 4;
    y[1] = digit / 5 * 8;
    digit = value % 10;
    x[2] = digit % 5 * 4;
    y[2] = digit / 5 * 8;
    for (i = 0; i < 3; i++) {
        CopyRectToBgTilemapRect(work->bgConfig, 6, i * 4 + 16, 11, 4, 8, work->screenData->rawData, x[i], y[i], 20, 16);
    }
    ScheduleBgTilemapBufferTransfer(work->bgConfig, 6);
}
