#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_72_wfc_connect.h"

extern u32 ov72_0223B46C;

void ov72_0223ACA8(NintendoWfcConnect *param_1);

void ov72_0223AD20(Window *arg0, MsgData *arg1, s32 arg2);

void ov72_0223ACA8(NintendoWfcConnect *param_1) {
    u32 baseTile;
    u32 *puVar1;
    Window *window;
    s32 iVar2;

    puVar1 = (u32 *)&ov72_0223B46C;
    iVar2 = 0;
    baseTile = 0x1f;
    window = param_1->buttonWindows;
    do {
        AddWindowParameterized(param_1->bgConfig, window, 4, (u8)*puVar1, 0x15, 8, 2, 0xd, baseTile);
        baseTile = baseTile + 0x40;
        puVar1 = puVar1 + 1;
        iVar2 = iVar2 + 1;
        window = window + 1;
    } while (iVar2 < 3);
    AddWindowParameterized(param_1->bgConfig, &param_1->numberInputWindow, 4, 2, 0x13, 0x1b, 4, 0xd, 0x4f);
    return;
}

void ov72_0223AD20(Window *window, MsgData *msgData, s32 strno) {
    String *string;

    string = NewString_ReadMsgData(msgData, strno);
    FillWindowPixelBuffer(window, 0U);
    AddTextPrinterParameterizedWithColor(window, FONT_UNK_4, string, 0U, 0U, 0U, 0xF0200U, NULL);
    CopyWindowToVram(window);
    String_Delete(string);
}
