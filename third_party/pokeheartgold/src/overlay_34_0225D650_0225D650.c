#include "global.h"

#include "bg_window.h"
#include "overlay_34.h"

void ov34_0225D650(BgConfig *bgConfig, Ov34Row *rows, Window *titleWindow) {
    s32 baseTile0;
    s32 baseTile1;
    s32 top1;
    s32 baseTile2;
    Ov34Row *row;
    s32 i;
    u32 top0;

    baseTile0 = 0xA0;
    baseTile1 = 0xD0;
    top1 = 5;
    row = rows;
    i = 0;
    top0 = 3;
    baseTile2 = 0x265;
    do {
        AddWindowParameterized(bgConfig, &row->window0, (u8)(i + 4), 1U, (u8)(s32)top0, 8U, 2U, 0xCU, (u16)(u16)baseTile0);
        FillWindowPixelBuffer(&row->window0, 0U);
        CopyWindowToVram(&row->window0);
        AddWindowParameterized(bgConfig, &row->window1, (u8)(i + 4), 2U, (u8)(u8)top1, 0x1BU, 5U, 0xCU, (u16)(u16)baseTile1);
        FillWindowPixelBuffer(&row->window1, 0U);
        CopyWindowToVram(&row->window1);
        AddWindowParameterized(bgConfig, &row->window2, (u8)(i + 4), 0xCU, (u8)(s32)top0, 0xFU, 2U, 0xCU, (u16)(s32)(s32)baseTile2);
        FillWindowPixelBuffer(&row->window2, 0U);
        CopyWindowToVram(&row->window2);
        i += 1;
        baseTile0 += 0x10;
        top0 += 7;
        baseTile1 += 0x87;
        row++;
        top1 += 7;
        baseTile2 += 0x1E;
    } while (i < 3);
    AddWindowParameterized(bgConfig, titleWindow, 4U, 8U, 0U, 7U, 2U, 0xCU, 0x2BFU);
    FillWindowPixelBuffer(titleWindow, 0U);
}
