#include "global.h"

#include "bg_window.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_0208805C.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "pokemon_types_def.h"
#include "battle/battle_party_context.h"
#include "overlay_08_battle_party.h"
#include "unk_0200CE7C.h"
#include "message_printer.h"
#include "string_util.h"
#include "msgdata.h"

struct MessagePrinter;

enum NonNumericSpecialChar {
    SPECIAL_CHAR_SLASH = 0,
    SPECIAL_CHAR_LEVEL,
    SPECIAL_CHAR_NUMBER,
    SPECIAL_CHAR_ID,
    SPECIAL_CHAR_LEVEL_FEMALE_UNUSED,
    SPECIAL_CHAR_LEVEL_MALE_UNUSED,
    SPECIAL_CHAR_LEVEL_UNUSED,
};

void ov08_0221DF80(BattleParty *battleParty, u32 windowIndex, u16 partyIndex, u8 x, u8 y);

void ov08_0221DF80(BattleParty *battleParty, u32 windowIndex, u16 partyIndex, u8 x, u8 y) {
    BattlePartyPokemon *pokemon = &battleParty->partyPokemon[partyIndex];

    FontSpecialChars_DrawPartyScreenText(battleParty->unused3, SPECIAL_CHAR_LEVEL, pokemon->level, 3, PRINTING_MODE_LEFT_ALIGN, &battleParty->windows[windowIndex], x + 8, y);
    ScheduleWindowCopyToVram(&battleParty->windows[windowIndex]);
}

void ov08_0221DFC8(BattleParty *battleParty, u32 windowIndex, u16 partyIndex, u8 x, u8 y);

void ov08_0221DFC8(BattleParty *battleParty, u32 windowIndex, u16 partyIndex, u8 x, u8 y) {
    BattlePartyPokemon *pokemon = &battleParty->partyPokemon[partyIndex];

    PrintUIntOnWindow(battleParty->unused3, pokemon->curHP, 3, PRINTING_MODE_RIGHT_ALIGN, &battleParty->windows[windowIndex], x, y);
    sub_0200CDAC(battleParty->unused3, SPECIAL_CHAR_SLASH, &battleParty->windows[windowIndex], x + 8 * 3, y);
    PrintUIntOnWindow(battleParty->unused3, pokemon->maxHP, 3, PRINTING_MODE_LEFT_ALIGN, &battleParty->windows[windowIndex], x + 8 * 3 + 8, y);
    ScheduleWindowCopyToVram(&battleParty->windows[windowIndex]);
}

enum BarColor {
    BARCOLOR_EMPTY = 0,
    BARCOLOR_RED,
    BARCOLOR_YELLOW,
    BARCOLOR_GREEN,

    BARCOLOR_MAX,
};

void ov08_0221E048(BattleParty *battleParty, u32 windowIndex, u16 partyIndex, u8 x, u8 y);

void ov08_0221E048(BattleParty *battleParty, u32 windowIndex, u16 partyIndex, u8 x, u8 y) {
    BattlePartyPokemon *pokemon = &battleParty->partyPokemon[partyIndex];
    u8 color = 1;
    u8 filledPixels = CalculateHpBarPixelsLength(pokemon->curHP, pokemon->maxHP, 48);

    switch (CalculateHpBarColor(pokemon->curHP, pokemon->maxHP, 48)) {
    case BARCOLOR_EMPTY:
        ScheduleWindowCopyToVram(&battleParty->windows[windowIndex]);
        return;
    case BARCOLOR_MAX:
    case BARCOLOR_GREEN:
        color = 1;
        break;
    case BARCOLOR_YELLOW:
        color = 3;
        break;
    case BARCOLOR_RED:
        color = 5;
        break;
    }

    FillWindowPixelRect(&battleParty->windows[windowIndex], color + 1, x, y + 1, filledPixels, 1);
    FillWindowPixelRect(&battleParty->windows[windowIndex], color, x, y + 2, filledPixels, 2);
    FillWindowPixelRect(&battleParty->windows[windowIndex], color + 1, x, y + 4, filledPixels, 1);
    ScheduleWindowCopyToVram(&battleParty->windows[windowIndex]);
}
