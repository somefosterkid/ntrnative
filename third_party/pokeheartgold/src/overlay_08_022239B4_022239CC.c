#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_08_battle_bag.h"

void ov08_022239CC(BattleBag *battleBag, u32 slot);

void ov08_022239CC(BattleBag *battleBag, u32 slot) {
    String *temp_r7;
    Window *temp_r4;

    temp_r4 = battleBag->windows;
    temp_r7 = NewString_ReadMsgData(battleBag->messageLoader, 9);
    BufferItemName(battleBag->stringTemplate, 0U, battleBag->items[battleBag->currentBattlePocket][slot].id);
    StringExpandPlaceholders(battleBag->stringTemplate, battleBag->string, temp_r7);
    FillWindowPixelBuffer(temp_r4, 0U);
    AddTextPrinterParameterizedWithColor(temp_r4, FONT_SYSTEM, battleBag->string, 0U, 0U, 0xFFU, 0x10200U, NULL);
    String_Delete(temp_r7);
    ScheduleWindowCopyToVram(temp_r4);
}
