#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_party_context.h"

typedef enum HeapID HeapID;

typedef enum Font Font;

void ov08_0221E244(BattleParty *battleParty, u32 move, u32 windowIndex, u32 textID, u16 font, u16 yOffset, u32 color);

void ov08_0221E244(BattleParty *battleParty, u32 move, u32 windowIndex, u32 textID, u16 font, u16 yOffset, u32 color) {
    Window *window = &battleParty->windows[windowIndex];
    String *formatted = String_New(16, battleParty->context->heapID);
    String *string = NewString_ReadMsgData(battleParty->messageLoader, textID);
    u32 xOffset;

    BufferMoveName(battleParty->stringTemplate, 0, move);
    StringExpandPlaceholders(battleParty->stringTemplate, formatted, string);

    if (font == 4) {
        xOffset = (GetWindowWidth(window) * 8 - FontID_String_GetWidth((Font)font, formatted, 0)) / 2;
    } else {
        xOffset = 0;
    }

    AddTextPrinterParameterizedWithColor(window, (Font)font, formatted, xOffset, yOffset, 0xff, color, NULL);
    String_Delete(string);
    String_Delete(formatted);
    ScheduleWindowCopyToVram(window);
}
