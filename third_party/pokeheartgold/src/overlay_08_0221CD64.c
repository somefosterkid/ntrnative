#include "global.h"

#include "font.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "systask_environment.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "pokemon_types_def.h"
#include "battle/battle_party_context.h"
#include "message_printer.h"
#include "msgdata.h"

struct BattlePartyContext;

struct BattleSubMenuCursor;

struct MessagePrinter;

struct MsgData;

struct Pokemon;

struct MessageFormat;

u8 ov08_0221CD64(BattleParty *battleParty);

u8 ov08_0221CD64(BattleParty *battleParty) {
    PaletteData_BeginPaletteFade(battleParty->palette, (0x2 | 0x8), 0xffff, -8, 0, 16, 0);
    return 26;
}

s32 ov08_0221CF08(BgConfig *);

s32 ov08_0221D14C(void *);

s32 ov08_0221DD40(void *);

s32 ov08_02220A50(void *);

SysTask *ov08_02224B7C(void *);

BOOL ov08_02224B8C(void *);

BOOL ov08_0221CD90(SysTask *task, BattleParty *battleParty);

BOOL ov08_0221CD90(SysTask *task, BattleParty *battleParty) {
    if (PaletteData_GetSelectedBuffersBitmask(battleParty->palette) != 0) {
        return 0;
    }
    ov08_0221D14C(battleParty);
    ov08_02220A50(battleParty);
    ov08_0221DD40(battleParty);
    ov08_0221CF08(battleParty->background);
    battleParty->context->isCursorEnabled = ov08_02224B8C(battleParty->cursor);
    ov08_02224B7C(battleParty->cursor);
    FontID_Release(FONT_UNK_4);
    battleParty->context->battlePartyExited = 1;
    DestroySysTaskAndEnvironment(task);
    return 1;
}
