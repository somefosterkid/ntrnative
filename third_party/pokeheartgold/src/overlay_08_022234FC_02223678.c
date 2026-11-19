#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"

#include "overlay_08_battle_bag.h"

extern u32 ov08_02225BE4[];

void ov08_02223678(BattleBag *battleBag, u32 itemIndex, u32 slot, u32 windowIndex, enum Font font, u32 yOffset, u32 color);

void ov08_02223678(BattleBag *battleBag, u32 itemIndex, u32 slot, u32 windowIndex, enum Font font, u32 yOffset, u32 color) {
    String *src;
    Window *windows;

    windows = battleBag->windows;
    FillWindowPixelBuffer(&windows[windowIndex], 0);
    if (battleBag->items[battleBag->currentBattlePocket][itemIndex].quantity != 0) {
        src = NewString_ReadMsgData(*((MsgData **)&battleBag->messageLoader), ov08_02225BE4[slot * 2]);
        BufferIntegerAsString(*((MessageFormat **)&battleBag->stringTemplate), 0, battleBag->items[battleBag->currentBattlePocket][itemIndex].quantity, 3, PRINTING_MODE_LEFT_ALIGN, 1);
        StringExpandPlaceholders(*((MessageFormat **)&battleBag->stringTemplate), battleBag->string, src);
        AddTextPrinterParameterizedWithColor(&windows[windowIndex], font, battleBag->string, 0, yOffset, 0xff, color, (PrinterCallback_t)0x0);
        String_Delete(src);
    }
    ScheduleWindowCopyToVram(&windows[windowIndex]);
    return;
}
