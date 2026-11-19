#include "global.h"
#include "overlay_108_021E8850.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "text.h"
#include "overlay_108_safari_decoration_app.h"

void ov108_021EA260(SafariDecorationApp *arg0) {
    s32 x;

    FillWindowPixelBuffer(&arg0->windows[4], 0U);
    ReadMsgDataIntoString(arg0->msgData, arg0->objects[arg0->selected].id + 0xE, arg0->strBuf1);
    x = 0x70 - FontID_String_GetWidth(FONT_UNK_4, arg0->strBuf1, 0U);
    AddTextPrinterParameterizedWithColor(&arg0->windows[4], FONT_UNK_4, arg0->strBuf1, x / 2, 0U, 0xFFU, 0x10200U, NULL);
    ScheduleWindowCopyToVram(&arg0->windows[4]);
    ClearWindowTilemapAndScheduleTransfer(&arg0->windows[3]);
}
