#include "global.h"

#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "touchscreen.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "pokemon_types_def.h"
#include "screen_fade_internal.h"
#include "battle/battle_party_context.h"
#include "message_printer.h"
#include "msgdata.h"

struct BattlePartyContext;

struct BattleSubMenuCursor;

struct BgConfig;

struct MessagePrinter;

struct MsgData;

struct Pokemon;

struct MessageFormat;

struct SysTaskQueue;

typedef void (*UnkCallback)(void);

enum ButtonMode {
    BUTTON_MODE_NORMAL = 0,
    BUTTON_MODE_START_IS_X = 1,
    BUTTON_MODE_SWAP_XY = 2,
    BUTTON_MODE_L_IS_A = 3,
};



u8 ov08_0221CA50(BattleParty *battleParty);

u8 ov08_0221CA50(BattleParty *battleParty) {
    if ((gSystem.newKeys & (0x0001 | 0x0002)) || System_GetTouchNew() == 1) {
        return battleParty->queuedState;
    }

    return 18;
}

u8 ov08_0221CA78(BattleParty *battleParty);

u8 ov08_0221CA78(BattleParty *battleParty) {
    if (battleParty->isAButtonPressed == 0) {
        return battleParty->queuedState;
    }

    return 22;
}
