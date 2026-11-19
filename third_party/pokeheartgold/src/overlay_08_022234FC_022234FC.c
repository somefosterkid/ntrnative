#include "global.h"

#include "battle/battle_bag_context.h"
#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "overlay_08_battle_bag.h"
#include "pm_string.h"
#include "text.h"

void ov08_022234A4(BattleBag *battleBag, u8 windowIndex, u32 textID, enum Font font, u32 yOffset, u32 color);

void ov08_022234FC(BattleBag *battleBag);

void ov08_022234FC(BattleBag *battleBag) {
    String *string;
    u32 i;

    for (i = 0; i < 5; i++) {
        FillWindowPixelBuffer(&battleBag->windows[i], 0);
    }
    ov08_022234A4(battleBag, 0, 0, FONT_UNK_4, 7, 0x30201);
    ov08_022234A4(battleBag, 0, 1, FONT_UNK_4, 0x17, 0x30201);
    ov08_022234A4(battleBag, 1, 2, FONT_UNK_4, 7, 0x30201);
    ov08_022234A4(battleBag, 1, 3, FONT_UNK_4, 0x17, 0x30201);
    ov08_022234A4(battleBag, 2, 7, FONT_UNK_4, 7, 0x30201);
    ov08_022234A4(battleBag, 3, 6, FONT_UNK_4, 7, 0x30201);
    if (battleBag->context->lastUsedItem != 0) {
        string = NewString_ReadMsgData(battleBag->messageLoader, 8);
        AddTextPrinterParameterizedWithColor(&battleBag->windows[4], FONT_UNK_4, string, 0, 5, 0xff, 0x30201, NULL);
        String_Delete(string);
        ScheduleWindowCopyToVram(&battleBag->windows[4]);
    }
}
