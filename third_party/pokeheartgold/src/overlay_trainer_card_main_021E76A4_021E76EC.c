#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "pm_string.h"
#include "text.h"
#include "trainer_card_main.h"

void ov51_021E76EC(TrainerCardMain *cardMain, int mode) {
    Window *window;
    u32 width;
    int font;
    int showInfo;
    font = 4;
    window = &cardMain->windows[12];
    if (mode == 0) {
        showInfo = cardMain->isConnected;
        if (showInfo != 0) {
            mode = 1;
        }
    }
    switch (mode) {
    case 0:
        FillWindowPixelBuffer(window, 0U);
        width = (u32)(0x60 - FontID_String_GetWidth((enum Font)font, cardMain->buttonStrings[2], 0U));
        AddTextPrinterParameterizedWithColor(window, (enum Font)4, cardMain->buttonStrings[2], width >> 1, 4, 0U, 0x50400U, 0L);
        ov51_021E7D44(&cardMain->sprites, 1, 2, 1);
        return;

    case 1:
        FillWindowPixelBuffer(window, 0U);
        AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, cardMain->buttonStrings[1], 0U, 4, 0U, 0x50400U, 0L);
        ov51_021E7D44(&cardMain->sprites, 1, 4, 1);
        return;
    }
}
