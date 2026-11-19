#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "render_window.h"
#include "text.h"

String *ov48_0225B0C4(void *, s32);

void ov48_02259D00(void *arg0, BgConfig **arg1, void *arg2, enum HeapID arg3);

void ov48_02259D00(void *arg0, BgConfig **arg1, void *arg2, enum HeapID heapID) {
    String *string;

    memset(arg0, 0, 0x2CU);
    AddWindowParameterized(*arg1, arg0 + 0x1C, 1U, 0x19U, 0x15U, 6U, 2U, 1U, 0x28U);
    FillWindowPixelBuffer(arg0 + 0x1C, 0xFU);
    string = ov48_0225B0C4(arg2, 1);
    FontID_Alloc(FONT_SUBSCREEN, heapID);
    AddTextPrinterParameterizedWithColor(arg0 + 0x1C, FONT_SUBSCREEN, string, FontID_String_GetCenterAlignmentX(FONT_SUBSCREEN, string, 0U, 0x30U), 0U, 0xFFU, 0x2010FU, NULL);
    FontID_Release(FONT_SUBSCREEN);
    DrawFrameAndWindow1(arg0 + 0x1C, 0, 0x1FU, 0U);
}
