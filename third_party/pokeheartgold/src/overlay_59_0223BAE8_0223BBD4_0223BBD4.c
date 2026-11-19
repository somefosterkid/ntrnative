#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "overlay_59_juice_feed_app.h"
#include "pm_string.h"
#include "text.h"

void ov59_0223BBD4(Ov59JuiceFeedApp *param_1, s32 param_2) {
    u8 uVar1;
    u32 uVar2;
    u32 textSpeed;

    switch (param_2) {
    case 0:
        uVar2 = param_1->ranks.ranks[0].flavor;
        if (uVar2 == 6) {
            param_2 = 0xc;
        } else if (uVar2 == 5) {
            param_2 = param_1->ranks.order[0] + 0xd;
        } else {
            param_2 = uVar2 + 0xd;
        }
        textSpeed = 0;
        break;
    case 1:
        param_2 = 0x12;
        textSpeed = (u32) param_1->textFrameDelay;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        param_2 = param_2 + 0x11;
        textSpeed = (u32) param_1->textFrameDelay;
        break;
    case 7:
    case 8:
    case 9:
    case 10:
    case 0xb:
        param_2 = param_2 + 0x11;
        textSpeed = (u32) param_1->textFrameDelay;
        break;
    default:
        textSpeed = (u32) param_1->textFrameDelay;
    }
    ReadMsgDataIntoString(param_1->msgData, param_2, param_1->unk_68);
    uVar1 = AddTextPrinterParameterizedWithColor(&param_1->windows[0], FONT_MESSAGE, param_1->unk_68, 0, 0, textSpeed, 0x1020f, (PrinterCallback_t)0x0);
    param_1->printerId = uVar1;
    if (textSpeed == 0) {
        ScheduleWindowCopyToVram(&param_1->windows[0]);
    }
    return;
}
