#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "bg_window.h"
#include "msgdata.h"
#include "render_window.h"
#include "text.h"

u16 ov59_02239E0C(Ov59ApricornApp *, Ov59JuiceRank *, s32);

void ov59_02238D90(Ov59ApricornApp *arg0);

void ov59_02238D90(Ov59ApricornApp *arg0) {
    u8 temp_r0;
    Window *window = &arg0->windows[0];

    FillWindowPixelBuffer(window, 0xFU);
    DrawFrameAndWindow2(window, 1, 1U, 0xDU);
    ReadMsgDataIntoString(arg0->msgData, ov59_02239E0C(arg0, &arg0->ranks[0], 0), arg0->unk_68);
    AddTextPrinterParameterized(window, FONT_MESSAGE, arg0->unk_68, 0U, 0U, 0xFFU, NULL);
    if (arg0->ranks[0].lo == 5) {
        ScheduleWindowCopyToVram(window);
        return;
    }
    if ((u32) arg0->rankCount > 1U) {
        ReadMsgDataIntoString(arg0->msgData, ov59_02239E0C(arg0, &arg0->ranks[1], 1), arg0->unk_68);
        AddTextPrinterParameterized(window, FONT_MESSAGE, arg0->unk_68, 0U, 0x10U, 0xFFU, NULL);
    }
    temp_r0 = arg0->rankCount;
    if ((u32)temp_r0 > 2U) {
        ReadMsgDataIntoString(arg0->msgData, ov59_02239E0C(arg0, &arg0->ranks[2], 2), arg0->unk_68);
        AddTextPrinterParameterized(window, FONT_MESSAGE, arg0->unk_68, 0U, 0x20U, 0xFFU, NULL);
    } else if ((u32)(u8)(temp_r0 + 0xFF) <= 1U) {
        ReadMsgDataIntoString(arg0->msgData, 0x7A, arg0->unk_68);
        AddTextPrinterParameterized(window, FONT_MESSAGE, arg0->unk_68, 0U, ((arg0->rankCount - 1) * 0x10) + 0x10, 0xFFU, NULL);
    }
    ScheduleWindowCopyToVram(window);
}
