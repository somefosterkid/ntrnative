#include "global.h"

#include "bg_window.h"
#include "overlay_96_ov96hurdle_hud.h"

typedef struct Ov96DigitPos {
    u8 y[3];
    u8 x[3];
} Ov96DigitPos;

void ov96_02208374(Ov96HurdleHud *hud, s32 value);

void ov96_02208374(Ov96HurdleHud *hud, s32 value) {
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
        CopyRectToBgTilemapRect(hud->bgConfig, 5, i * 4 + 14, 3, 4, 8, hud->scrnData->rawData, x[i], y[i], 20, 16);
    }
    ScheduleBgTilemapBufferTransfer(hud->bgConfig, 5);
}
