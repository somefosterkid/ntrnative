#include "global.h"

#include "battle/battle_cursor.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_party_context.h"

BattleCursor *ov08_02224B84(void *);

s32 ov08_02224B90(void *, s32);

s32 ov08_02224BC0(void *);

void ov08_022217C8(BattleParty *battleParty);

void ov08_022217C8(BattleParty *battleParty) {
    ov08_02224B90(battleParty->cursor, 0);
    ov08_02224BC0(battleParty->cursor);
    BattleCursor_Disable(ov08_02224B84(battleParty->cursor));
}

struct BattlePartyContext;

struct BattleSubMenuCursor;

struct MessagePrinter;

struct MsgData;

struct MessageFormat;

static void ov08_02221BD0(u16 *buttonData, u16 *screenData, u8 xOffset, u8 yOffset, u8 width, u8 height);

void ov08_022217F0(BattleParty *battleParty, u16 *screenData);

void ov08_022217F0(BattleParty *battleParty, u16 *screenData) {
    ov08_02221BD0(battleParty->partyPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_UNPRESSED], screenData, 0, 0, 16, 6);
    ov08_02221BD0(battleParty->partyPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_PRESSING], screenData, 0, 6, 16, 6);
    ov08_02221BD0(battleParty->partyPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_PRESSED], screenData, 0, 6 * 2, 16, 6);
    ov08_02221BD0(battleParty->partyPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_DISABLED], screenData, 0, 6 * 3, 16, 6);

    ov08_02221BD0(battleParty->altPartyPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_UNPRESSED], screenData, 16, 0, 16, 6);
    ov08_02221BD0(battleParty->altPartyPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_PRESSING], screenData, 16, 6, 16, 6);
    ov08_02221BD0(battleParty->altPartyPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_PRESSED], screenData, 16, 6 * 2, 16, 6);
    ov08_02221BD0(battleParty->altPartyPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_DISABLED], screenData, 16, 6 * 3, 16, 6);

    ov08_02221BD0(battleParty->summaryCheckMovesButtonData[BATTLE_SUB_MENU_BUTTON_STATE_UNPRESSED], screenData, 0, 39, 13, 5);
    ov08_02221BD0(battleParty->summaryCheckMovesButtonData[BATTLE_SUB_MENU_BUTTON_STATE_PRESSING], screenData, 0, 39 + 5, 13, 5);
    ov08_02221BD0(battleParty->summaryCheckMovesButtonData[BATTLE_SUB_MENU_BUTTON_STATE_PRESSED], screenData, 13, 39, 13, 5);
    ov08_02221BD0(battleParty->summaryCheckMovesButtonData[BATTLE_SUB_MENU_BUTTON_STATE_DISABLED], screenData, 13, 39 + 5, 13, 5);

    ov08_02221BD0(battleParty->prevPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_UNPRESSED], screenData, 0, 49, 5, 5);
    ov08_02221BD0(battleParty->prevPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_PRESSING], screenData, 5, 49, 5, 5);
    ov08_02221BD0(battleParty->prevPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_PRESSED], screenData, 5 * 2, 49, 5, 5);
    ov08_02221BD0(battleParty->prevPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_DISABLED], screenData, 5 * 3, 49, 5, 5);

    ov08_02221BD0(battleParty->nextPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_UNPRESSED], screenData, 0, 54, 5, 5);
    ov08_02221BD0(battleParty->nextPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_PRESSING], screenData, 5, 54, 5, 5);
    ov08_02221BD0(battleParty->nextPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_PRESSED], screenData, 5 * 2, 54, 5, 5);
    ov08_02221BD0(battleParty->nextPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_DISABLED], screenData, 5 * 3, 54, 5, 5);

    ov08_02221BD0(battleParty->cancelButtonData[BATTLE_SUB_MENU_BUTTON_STATE_UNPRESSED], screenData, 26, 24, 5, 5);
    ov08_02221BD0(battleParty->cancelButtonData[BATTLE_SUB_MENU_BUTTON_STATE_PRESSING], screenData, 26, 24 + 5, 5, 5);
    ov08_02221BD0(battleParty->cancelButtonData[BATTLE_SUB_MENU_BUTTON_STATE_PRESSED], screenData, 26, 24 + 5 * 2, 5, 5);
    ov08_02221BD0(battleParty->cancelButtonData[BATTLE_SUB_MENU_BUTTON_STATE_DISABLED], screenData, 26, 24 + 5 * 3, 5, 5);

    ov08_02221BD0(battleParty->learnMoveConfirmButtonData[BATTLE_SUB_MENU_BUTTON_STATE_UNPRESSED], screenData, 0, 24, 26, 5);
    ov08_02221BD0(battleParty->learnMoveConfirmButtonData[BATTLE_SUB_MENU_BUTTON_STATE_PRESSING], screenData, 0, 24 + 5, 26, 5);
    ov08_02221BD0(battleParty->learnMoveConfirmButtonData[BATTLE_SUB_MENU_BUTTON_STATE_PRESSED], screenData, 0, 24 + 5 * 2, 26, 5);

    ov08_02221BD0(battleParty->contestStatsButtonData[BATTLE_SUB_MENU_BUTTON_STATE_UNPRESSED], screenData, 20, 49, 9, 4);
    ov08_02221BD0(battleParty->contestStatsButtonData[BATTLE_SUB_MENU_BUTTON_STATE_PRESSING], screenData, 20, 49 + 4, 9, 4);
    ov08_02221BD0(battleParty->contestStatsButtonData[BATTLE_SUB_MENU_BUTTON_STATE_PRESSED], screenData, 20, 49 + 4 * 2, 9, 4);

    ov08_02221BD0(battleParty->moveSummaryScreenMoveButtonData[BATTLE_SUB_MENU_BUTTON_STATE_UNPRESSED], screenData, 0, 59, 5, 2);
    ov08_02221BD0(battleParty->moveSummaryScreenMoveButtonData[BATTLE_SUB_MENU_BUTTON_STATE_PRESSING], screenData, 5, 59, 5, 2);
    ov08_02221BD0(battleParty->moveSummaryScreenMoveButtonData[BATTLE_SUB_MENU_BUTTON_STATE_PRESSED], screenData, 5 * 2, 59, 5, 2);
}

void ov08_02221B1C(BattleParty *battleParty, u16 *screenData);

void ov08_02221B1C(BattleParty *battleParty, u16 *screenData) {
    ov08_02221BD0(battleParty->shiftPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_UNPRESSED], screenData, 0, 0, 30, 17);
    ov08_02221BD0(battleParty->shiftPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_PRESSING], screenData, 0, 17, 30, 17);
    ov08_02221BD0(battleParty->shiftPokemonButtonData[BATTLE_SUB_MENU_BUTTON_STATE_PRESSED], screenData, 0, 17 * 2, 30, 17);

    ov08_02221BD0(battleParty->moveButtonData[BATTLE_SUB_MENU_BUTTON_STATE_UNPRESSED], screenData, 0, 51, 16, 6);
    ov08_02221BD0(battleParty->moveButtonData[BATTLE_SUB_MENU_BUTTON_STATE_PRESSING], screenData, 16, 51, 16, 6);
    ov08_02221BD0(battleParty->moveButtonData[BATTLE_SUB_MENU_BUTTON_STATE_PRESSED], screenData, 0, 51 + 6, 16, 6);
    ov08_02221BD0(battleParty->moveButtonData[BATTLE_SUB_MENU_BUTTON_STATE_DISABLED], screenData, 16, 51 + 6, 16, 6);
}

static void ov08_02221BD0(u16 *buttonData, u16 *screenData, u8 xOffset, u8 yOffset, u8 width, u8 height) {
    for (u16 i = 0; i < height; i++) {
        for (u16 l = 0; l < width; l++) {
            buttonData[i * width + l] = screenData[(yOffset + i) * (8 * (8 / 2)) + xOffset + l];
        }
    }
}

s32 ov08_02221C20(s32 arg0, u32 arg1, s32 arg2, s32 arg3);

s32 ov08_02221C20(s32 arg0, u32 arg1, s32 arg2, s32 arg3) {
    switch (arg1) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        if (arg3 == 0) {
            return arg0 + 0x1EC + (0xC0 * arg2);
        }
        return arg0 + 0x4EC + (0xC0 * arg2);
    case 6:
        return arg0 + 0xB84 + (0x32 * arg2);
    case 7:
        return arg0 + 0xC4C + (0x3FC * arg2);
    case 8:
    case 9:
    case 10:
    case 11:
        return arg0 + 0x7EC + (0x82 * arg2);
    case 12:
        return arg0 + 0x9F4 + (0x32 * arg2);
    case 13:
        return arg0 + 0xABC + (0x32 * arg2);
    case 14:
    case 15:
    case 16:
    case 17:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
        return arg0 + 0x1840 + (0xC0 * arg2);
    case 28:
    case 29:
        return arg0 + 0x1B40 + (arg2 * 0x104);
    case 18:
        return arg0 + 0x1E4C + (0x48 * arg2);
    case 30:
    case 31:
    case 32:
    case 33:
        return arg0 + 0x1F24 + (0x14 * arg2);
    default:
        return 0;
    }
}
