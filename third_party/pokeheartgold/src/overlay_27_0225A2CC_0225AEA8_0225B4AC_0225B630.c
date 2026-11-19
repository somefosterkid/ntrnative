#include "global.h"

#include "bg_window.h"
#include "error_handling.h"

void ov27_0225B630(Window *arg0, s32 arg1);

void ov27_0225B630(Window *arg0, s32 arg1) {
    BgConfig *bgConfig;
    s32 x;
    s32 y;
    s32 width;
    s32 height;
    s32 xw;

    bgConfig = arg0->bgConfig;
    x = GetWindowX(arg0);
    y = GetWindowY(arg0);
    width = GetWindowWidth(arg0);
    height = GetWindowHeight(arg0);
    switch (arg1) {
    case 1:
        FillBgTilemapRect(bgConfig, 4, 0xA9, (u8)(x - 3), (u8)(y - 1), 1, 1, 2);
        FillBgTilemapRect(bgConfig, 4, 0xAA, (u8)(x - 2), (u8)(y - 1), 1, 1, 2);
        FillBgTilemapRect(bgConfig, 4, 0xAB, (u8)(x - 1), (u8)(y - 1), 1, 1, 2);
        FillBgTilemapRect(bgConfig, 5, 0xAC, (u8)x, (u8)(y - 1), (u8)width, 1, 2);
        xw = x + width;
        FillBgTilemapRect(bgConfig, 5, 0xAD, (u8)xw, (u8)(y - 1), 1, 1, 2);
        FillBgTilemapRect(bgConfig, 4, 0x64, (u8)(x - 3), (u8)y, 1, 1, 2);
        FillBgTilemapRect(bgConfig, 4, 0x65, (u8)(x - 2), (u8)y, 1, 1, 2);
        FillBgTilemapRect(bgConfig, 4, 0x66, (u8)(x - 1), (u8)y, 1, 1, 2);
        FillBgTilemapRect(bgConfig, 5, 0x68, (u8)xw, (u8)y, 1, 1, 2);
        FillBgTilemapRect(bgConfig, 4, 0x84, (u8)(x - 3), (u8)(y + 1), 1, 1, 2);
        FillBgTilemapRect(bgConfig, 4, 0x85, (u8)(x - 2), (u8)(y + 1), 1, 1, 2);
        FillBgTilemapRect(bgConfig, 4, 0x86, (u8)(x - 1), (u8)(y + 1), 1, 1, 2);
        FillBgTilemapRect(bgConfig, 5, 0x88, (u8)xw, (u8)(y + 1), 1, 1, 2);
        y += height;
        FillBgTilemapRect(bgConfig, 4, 0xA4, (u8)(x - 3), (u8)y, 1, 1, 2);
        FillBgTilemapRect(bgConfig, 4, 0xA5, (u8)(x - 2), (u8)y, 1, 1, 2);
        FillBgTilemapRect(bgConfig, 4, 0xA6, (u8)(x - 1), (u8)y, 1, 1, 2);
        FillBgTilemapRect(bgConfig, 5, 0xA7, (u8)x, (u8)y, (u8)width, 1, 2);
        FillBgTilemapRect(bgConfig, 5, 0xA8, (u8)xw, (u8)y, 1, 1, 2);
        break;
    case 2:
        FillBgTilemapRect(bgConfig, 4, 7, (u8)(x - 3), (u8)(y - 1), 1, 1, 0);
        FillBgTilemapRect(bgConfig, 4, 7, (u8)(x - 2), (u8)(y - 1), 1, 1, 0);
        FillBgTilemapRect(bgConfig, 4, 7, (u8)(x - 1), (u8)(y - 1), 1, 1, 0);
        FillBgTilemapRect(bgConfig, 5, 0, (u8)x, (u8)(y - 1), (u8)width, 1, 2);
        xw = x + width;
        FillBgTilemapRect(bgConfig, 5, 0, (u8)xw, (u8)(y - 1), 1, 1, 2);
        FillBgTilemapRect(bgConfig, 4, 7, (u8)(x - 3), (u8)y, 1, 1, 0);
        FillBgTilemapRect(bgConfig, 4, 7, (u8)(x - 2), (u8)y, 1, 1, 0);
        FillBgTilemapRect(bgConfig, 4, 7, (u8)(x - 1), (u8)y, 1, 1, 0);
        FillBgTilemapRect(bgConfig, 5, 0, (u8)xw, (u8)y, 1, 1, 2);
        FillBgTilemapRect(bgConfig, 4, 7, (u8)(x - 3), (u8)(y + 1), 1, 1, 0);
        FillBgTilemapRect(bgConfig, 4, 7, (u8)(x - 2), (u8)(y + 1), 1, 1, 0);
        FillBgTilemapRect(bgConfig, 4, 7, (u8)(x - 1), (u8)(y + 1), 1, 1, 0);
        FillBgTilemapRect(bgConfig, 5, 0, (u8)xw, (u8)(y + 1), 1, 1, 2);
        y += height;
        FillBgTilemapRect(bgConfig, 4, 7, (u8)(x - 3), (u8)y, 1, 1, 0);
        FillBgTilemapRect(bgConfig, 4, 7, (u8)(x - 2), (u8)y, 1, 1, 0);
        FillBgTilemapRect(bgConfig, 4, 7, (u8)(x - 1), (u8)y, 1, 1, 0);
        FillBgTilemapRect(bgConfig, 5, 0, (u8)x, (u8)y, (u8)width, 1, 2);
        FillBgTilemapRect(bgConfig, 5, 0, (u8)xw, (u8)y, 1, 1, 2);
        break;
    default:
        GF_AssertFail();
        break;
    }
    ScheduleBgTilemapBufferTransfer(bgConfig, 4);
    ScheduleBgTilemapBufferTransfer(bgConfig, 5);
}
