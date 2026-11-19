#include "global.h"
struct HallOfFameShowcase;

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "unk_02019BA4.h"
#include "overlay_64_hall_of_fame_showcase.h"

extern u32 ov64_021E7040;

void ov64_021E6170(HallOfFameShowcase *param_1);

extern GridCallbacks ov64_021E6EA8;
extern TouchscreenHitbox ov64_021E6F6C;
extern DpadMenuBox ov64_021E7008;
void ov64_021E622C(struct HallOfFameShowcase *callbackParam);

void ov64_021E6170(HallOfFameShowcase *param_1) {
    u32 uVar1;
    WindowTemplate *template;
    Window *window;
    u32 uVar2;

    FontID_Alloc(FONT_UNK_4, HEAP_ID_59);
    template = (WindowTemplate *)&ov64_021E7040;
    uVar2 = 0;
    window = param_1->windows;
    do {
        AddWindow(param_1->bgConfig, window, template);
        uVar2 = uVar2 + 1;
        template = template + 1;
        window = window + 1;
    } while (uVar2 < 0x11);
    FillWindowPixelBuffer(&param_1->windows[16], 0);
    ReadMsgDataIntoString(param_1->msgData, 6, param_1->stringBuf);
    uVar1 = FontID_String_GetWidth(FONT_UNK_4, param_1->stringBuf, 0);
    AddTextPrinterParameterizedWithColor(&param_1->windows[16], FONT_UNK_4, param_1->stringBuf, 0x18 - (uVar1 >> 1), 0, 0xff, 0xf0100, (PrinterCallback_t)0x0);
    CopyWindowPixelsToVram_TextMode(&param_1->windows[16]);
    ScheduleWindowCopyToVram(&param_1->windows[16]);
    return;
}

void ov64_021E620C(u8 *param_1);

void ov64_021E620C(u8 *param_1) {
    u32 uVar1;
    Window *window;

    uVar1 = 0;
    window = (Window *)(param_1 + 8);
    do {
        RemoveWindow(window);
        uVar1 = uVar1 + 1;
        window = window + 1;
    } while (uVar1 < 0x11);
    FontID_Release(FONT_UNK_4);
    return;
}

void ov64_021E622C(HallOfFameShowcase *callbackParam) {
    callbackParam->gridInput = GridInputHandler_Create(&ov64_021E6F6C, &ov64_021E7008, &ov64_021E6EA8, callbackParam, 1, 0U, HEAP_ID_59);
}
