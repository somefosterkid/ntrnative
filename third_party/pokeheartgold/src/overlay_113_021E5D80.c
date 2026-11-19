#include "global.h"
#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "sprite.h"
#include "text.h"
#include "overlay_113.h"

extern u16 ov113_021E6BD0;

u8 ov113_021E5D80(UnownReportData *arg0);

s32 ov113_021E5FC0(UnownReportData *);
s32 ov113_021E6084(UnownReportData *, s32);
s32 ov113_021E613C(UnownReportData *, s32);
s32 ov113_021E6B1C(UnownReportData *);
void ov113_021E5DA4(UnownReportData *arg0, u8 unused);
void ov113_021E5E64(UnownReportData *arg0, s32 arg1);

u8 ov113_021E5D80(UnownReportData *arg0) {
    s32 v0;
    u16 *v1;

    v1 = &ov113_021E6BD0;
    v0 = 0;
loop_1:
    if ((s32)((u32)(*v1 << 0x10) >> 0x18) <= (s32)arg0->reportLevel) {
        v0 += 1;
        v1 += 2;
        if (v0 < 6) {
            goto loop_1;
        }
    }
    return (u8)v0;
}

void ov113_021E5DA4(UnownReportData *arg0, u8 unused) { // callers pass the current page in r1; the body re-reads it from arg0
    s32 v0;
    u8 v1;
    u8 v2;

    FillWindowPixelBuffer(&arg0->windows[1], 0U);
    FillWindowPixelBuffer(&arg0->windows[2], 0U);
    FillWindowPixelBuffer(&arg0->windows[3], 0U);
    v2 = arg0->page;
    if (v2 == 0) {
        ov113_021E5FC0(arg0);
    } else {
        v1 = arg0->numFormPages;
        v0 = v2 - 1;
        if (v0 < (s32)v1) {
            ov113_021E6084(arg0, (u8)v0);
        } else {
            ov113_021E613C(arg0, (u8)(v2 - (v1 + 1)));
        }
    }
    BufferIntegerAsString(arg0->msgFormat, 0U, arg0->page + 1, 1U, PRINTING_MODE_LEFT_ALIGN, 1);
    BufferIntegerAsString(arg0->msgFormat, 1U, (s32)arg0->numPages, 1U, PRINTING_MODE_LEFT_ALIGN, 1);
    StringExpandPlaceholders(arg0->msgFormat, arg0->strBuf2, arg0->pageFormatString);
    AddTextPrinterParameterizedWithColor(&arg0->windows[3], FONT_SYSTEM, arg0->strBuf2, 0U, 0U, 0xFFU, 0xF0200U, NULL);
    ScheduleWindowCopyToVram(&arg0->windows[1]);
    ScheduleWindowCopyToVram(&arg0->windows[2]);
    ScheduleWindowCopyToVram(&arg0->windows[3]);
    ov113_021E6B1C(arg0);
}

void ov113_021E5E64(UnownReportData *arg0, s32 seq) {
    Sprite_SetAnimCtrlSeq(arg0->sprites[0], seq);
    Sprite_ResetAnimCtrlState(arg0->sprites[0]);
    FillWindowPixelBuffer(arg0->windows, 0U);
    AddTextPrinterParameterizedWithColor(arg0->windows, FONT_UNK_4, arg0->titleString, (u32)(0x30 - FontID_String_GetWidth(FONT_UNK_4, arg0->titleString, 0U)) >> 1, 0U, 0xFFU, 0x70F00U, NULL);
    ScheduleWindowCopyToVram(arg0->windows);
}
