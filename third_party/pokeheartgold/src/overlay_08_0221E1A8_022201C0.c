#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "palette.h"
#include "pm_string.h"
#include "render_window.h"
#include "sprite.h"
#include "sprite_system.h"
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

void ov08_022201E8(BattleParty *battleParty);

enum BattlePartyMessageBox {
    BATTLE_PARTY_MESSAGE_BOX_SELECT_OPTION = 0,
    BATTLE_PARTY_MESSAGE_BOX_ERROR,
};

void ov08_022201C0(BattleParty *battleParty);

void ov08_022201C0(BattleParty *battleParty) {
    DrawFrameAndWindow2(&battleParty->messageBoxWindows[BATTLE_PARTY_MESSAGE_BOX_ERROR], TRUE, 1, 14);
    FillWindowPixelBuffer(&battleParty->messageBoxWindows[BATTLE_PARTY_MESSAGE_BOX_ERROR], 15);
    ov08_022201E8(battleParty);
}
