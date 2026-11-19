#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"

u32 ov73_021E7818(void *);

void ov73_021E756C(void *arg0, s32 arg1, s32 arg2);

void ov73_021E761C(u8 *param_1);

void ov73_021E756C(void *arg0, s32 strno, s32 arg2) {
    String *string;

    string = String_New(0xB4U, HEAP_ID_50);
    ReadMsgDataIntoString(*(MsgData **)((u8 *)arg0 + 0x28), strno, string);
    StringExpandPlaceholders(*(MessageFormat **)((u8 *)arg0 + 0x24), *(String **)((u8 *)arg0 + 0x44), string);
    String_Delete(string);
    FillWindowPixelBuffer(arg0 + 0x2A8, 0xFU);
    DrawFrameAndWindow2(arg0 + 0x2A8, 0, 1U, 0xBU);
    if (arg2 == 0) {
        *(s32 *)((u8 *)arg0 + 0x4C) = (s32)AddTextPrinterParameterized(arg0 + 0x2A8, FONT_MESSAGE, *(String **)((u8 *)arg0 + 0x44), 0U, 0U, ov73_021E7818(arg0), NULL);
        return;
    }
    AddTextPrinterParameterized(arg0 + 0x2A8, FONT_MESSAGE, *(String **)((u8 *)arg0 + 0x44), 0U, 0U, 0U, NULL);
    *(s32 *)((u8 *)arg0 + 0x4C) = 0xFF;
}

int ov73_021E75FC(int param0);

int ov73_021E75FC(int param0) {
    if (param0 == 0xff) {
        return 1;
    }

    if (TextPrinterCheckActive(param0) == 0) {
        return 1;
    }

    return 0;
}

void ov73_021E761C(u8 *param_1) {
    ClearFrameAndWindow2((Window *)(param_1 + 0x2a8), 0);
    return;
}
