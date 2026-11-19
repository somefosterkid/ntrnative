#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_96_ov96hurdle_hud.h"
#include "pm_string.h"
#include "text.h"

void ov96_022088AC(Ov96HurdleHud *hud, s32 param_2);

void ov96_022088AC(Ov96HurdleHud *hud, s32 param_2) {
    String *string;

    FillWindowPixelBuffer(&hud->window, 0);
    BufferString(hud->msgFormat, 0, hud->names[param_2], 2, 1, 2);
    string = ReadMsgData_ExpandPlaceholders(hud->msgFormat, hud->msgData, 0x9a, hud->heapId);
    AddTextPrinterParameterizedWithColor(&hud->window, FONT_SYSTEM, string, 0, 0, 0xff, 0xf0e00, (PrinterCallback_t)0x0);
    String_Delete(string);
    CopyWindowToVram(&hud->window);
    return;
}
