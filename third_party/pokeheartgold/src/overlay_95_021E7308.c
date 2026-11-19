#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_95_egg_hatch_app.h"

void ov95_021E7308(EggHatchPrompt *param_1);

void ov95_021E7328(EggHatchPrompt *arg0, Window *arg1, u32 arg2);

void ov95_021E7308(EggHatchPrompt *param_1) {
    s32 iVar1;
    Window *window;

    if (param_1 == 0) {
        GF_AssertFail();
    }
    iVar1 = 0;
    window = param_1->windows;
    do {
        RemoveWindow(window);
        iVar1 = iVar1 + 1;
        window = window + 1;
    } while (iVar1 < 2);
    return;
}

void ov95_021E7328(EggHatchPrompt *arg0, Window *window, u32 msgno) {
    MsgData *msgData;
    String *string;

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xBB, arg0->heapID);
    string = ReadMsgData_ExpandPlaceholders(arg0->msgFormat, msgData, msgno, arg0->heapID);
    FillWindowPixelBuffer(window, 0U);
    AddTextPrinterParameterizedWithColor(window, FONT_UNK_4, string, 0U, 0U, 0U, 0x10200U, NULL);
    CopyWindowToVram(window);
    DestroyMsgData(msgData);
    String_Delete(string);
}
