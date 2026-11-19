#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "font_types_def.h"
#include "pm_string.h"
#include "string_util.h"
#include "text.h"
#include "trainer_card_main.h"

typedef u32 TextColor;

void ov51_021E7664(Window *window, BOOL showColon, String *string);

void ov51_021E76A4(void *arg0);

void ov51_021E7664(Window *window, BOOL showColon, String *string) {
    if (showColon) {
        AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, 205, 0, 0, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    } else {
        FillWindowPixelRect(window, 0, 205, 0, 5, 2 * 8);
        CopyWindowToVram(window);
    }
}

void ov51_021E76A4(void *arg0) {
    u32 v0;

    v0 = 0x30 - FontID_String_GetWidth(FONT_UNK_4, ((TrainerCardMain *)arg0)->buttonStrings[0], 0U);
    FillWindowPixelBuffer(&((TrainerCardMain *)arg0)->windows[13], 0U);
    AddTextPrinterParameterizedWithColor(&((TrainerCardMain *)arg0)->windows[13], FONT_UNK_4, ((TrainerCardMain *)arg0)->buttonStrings[0], v0 >> 1, 4U, 0U, 0x50400U, NULL);
}
