#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "font_types_def.h"
#include "pm_string.h"
#include "text.h"
#include "msgdata.h"
#include "render_window.h"
#include "yes_no_prompt.h"

typedef u32 TextColor;

void ov65_0221FB4C(Window *window, String *str, int unused, u32 textSpeed, int xOrCenter, int yOffset);

u8 ov65_0221FB90(Window *arg0, u32 arg1, enum Font arg2, MsgData *arg3, MessageFormat *arg4);

YesNoPrompt *ov65_0221FC08(BgConfig *arg0);

void ov65_0221FB4C(Window *window, String *str, int unused, u32 textSpeed, int xOrCenter, int yOffset) {
    int xOffset = 0;

    if (xOrCenter == 1) {
        int strWidth = FontID_String_GetWidth(FONT_SYSTEM, str, 0);
        xOffset = ((window->width * 8) - strWidth) / 2;
    } else {
        xOffset = xOrCenter;
    }

    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, str, xOffset, yOffset, textSpeed, ((TextColor)(((11 & 0xFF) << 16) | ((12 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
}

u8 ov65_0221FB90(Window *window, u32 msgno, enum Font fontId, MsgData *msgData, MessageFormat *messageFormat) {
    String *string;
    u8 v0;

    string = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, msgno, HEAP_ID_26);
    if (fontId == FONT_MESSAGE) {
        DrawFrameAndWindow2(window, 1, 0x3D9U, 0xAU);
    } else {
        DrawFrameAndWindow1(window, 1, 0x3F7U, 0xBU);
    }
    FillWindowPixelBuffer(window, 0xFU);
    v0 = AddTextPrinterParameterizedWithColor(window, fontId, string, 0U, 0U, 0xFFU, 0x1020FU, NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(string);
    return v0;
}

YesNoPrompt *ov65_0221FC08(BgConfig *arg0) {
    YesNoPromptTemplate dest;
    YesNoPrompt *yesNoPrompt;

    MI_CpuFill8(&dest, 0U, sizeof(YesNoPromptTemplate));
    dest.bgId = 0;
    dest.tileStart = 0x180;
    dest.plttSlot = 0xE;
    dest.bgConfig = arg0;
    dest.x = 0x1A;
    dest.y = 0xA;
    *(u8 *)((u8 *)(&dest) + 0x12) = (u8)(*(u8 *)((u8 *)(&dest) + 0x12) & ~0xF);
    *(u8 *)((u8 *)(&dest) + 0x12) = (u8)(*(u8 *)((u8 *)(&dest) + 0x12) & ~0xF0);
    yesNoPrompt = YesNoPrompt_Create(HEAP_ID_26);
    YesNoPrompt_InitFromTemplate(yesNoPrompt, (YesNoPromptTemplate *)&dest);
    return yesNoPrompt;
}
