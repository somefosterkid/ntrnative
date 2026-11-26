#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "overlay_86_frontier_records_app.h"
#include "pm_string.h"

void ov86_021E6E30(FrontierRecordsApp *param_1) {
    u8 bVar1;
    String *string;

    string = NewString_ReadMsgData(param_1->msgData, 0x27);
    FillWindowPixelBuffer(&param_1->buttonWindow, 0);
    bVar1 = GetWindowWidth(&param_1->buttonWindow);
    ov86_021E5FD8(&param_1->buttonWindow, string, (bVar1 * 8) / 2, 0, FONT_UNK_4, 0xf0100, 2);
    ScheduleWindowCopyToVram(&param_1->buttonWindow);
    String_Delete(string);
    return;
}
