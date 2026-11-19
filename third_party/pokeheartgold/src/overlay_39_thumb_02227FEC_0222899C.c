#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_39_thumb.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"
#include "overlay_39_unk_struct_ov61_0222_c664.h"

u32 ov39_02228964(Window *, String *, s32, s32, u32, s32);

void ov39_0222899C(Window *arg0, String *arg1, s32 arg2, u32 arg3, s32 arg4, u32 arg5);

void ov39_0222899C(Window *window, String *string, s32 arg2, u32 y, s32 arg4, u32 color) {
    AddTextPrinterParameterizedWithColor(window, FONT_MESSAGE, string, ov39_02228964(window, string, arg2, arg4, color, 1), y, 0U, color, NULL);
}

static void ov39_022289D0(void *arg0, s32 strno) {
    String *string;

    string = String_New(0x100U, HEAP_ID_124);
    ReadMsgDataIntoString(((MsgData *)((UnkStruct_ov61_0222C664 *)arg0)->unk_2C), strno, string);
    StringExpandPlaceholders(((MessageFormat *)((UnkStruct_ov61_0222C664 *)arg0)->unk_20), ((UnkStruct_ov61_0222C664 *)arg0)->unk_3C, string);
    FillWindowPixelBuffer(arg0 + 0x64, 0xFU);
    DrawFrameAndWindow1(arg0 + 0x64, 1, 0x1FU, 0xBU);
    AddTextPrinterParameterized(arg0 + 0x64, FONT_MESSAGE, ((UnkStruct_ov61_0222C664 *)arg0)->unk_3C, 0U, 0U, 0U, NULL);
    *((s32 *)&((UnkStruct_ov61_0222C664 *)arg0)->unk_40) = 0xFF;
    String_Delete(string);
}

void ov39_02228A34(void *arg0, s32 arg1, s32 arg2);

void ov39_02228A34(void *arg0, s32 arg1, s32 num) {
    s32 v0;

    v0 = arg1;
    if (v0 == -1) {
        v0 = 0xB;
    }
    BufferIntegerAsString(((MessageFormat *)((UnkStruct_ov61_0222C664 *)arg0)->unk_20), 0U, num, 5U, PRINTING_MODE_LEADING_ZEROS, 1);
    ClearFrameAndWindow2(arg0 + 0x44, 1);
    ov39_022289D0(arg0, v0);
}
