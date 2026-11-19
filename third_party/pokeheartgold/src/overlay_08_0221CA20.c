#include "global.h"

#include "palette.h"
#include "render_window.h"
#include "sprite.h"
#include "sprite_system.h"
#include "text.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle_party_context.h"
#include "message_printer.h"
#include "msgdata.h"

struct BattlePartyContext;

struct BattleSubMenuCursor;

struct MessagePrinter;

struct MsgData;

struct MessageFormat;

u8 ov08_0221CA20(BattleParty *battleParty);

u8 ov08_0221CA20(BattleParty *battleParty) {
    ClearFrameAndWindow2(&battleParty->messageBoxWindows[1], 0);
    return 2;
}

u8 ov08_0221CA34(BattleParty *battleParty);

u8 ov08_0221CA34(BattleParty *battleParty) {
    if (TextPrinterCheckActive(battleParty->textPrinterID) == 0) {
        return 18;
    }

    return 17;
}
