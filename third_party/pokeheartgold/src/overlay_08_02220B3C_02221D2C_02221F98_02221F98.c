#include "global.h"

#include "constants/heap.h"

#include "battle/battle.h"

#include "bg_window.h"
#include "filesystem.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "wild_encounter.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "msgdata.h"
#include "battle/battle_cursor.h"
#include "battle/battle_party_context.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;


typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct GridMenuCursorPosition GridMenuCursorPosition;

typedef u16 charcode_t;

enum BattleSelectPokemonScreenButton {
    BATTLE_POKEMON_SELECT_POKEMON_SCREEN_BUTTON_SHIFT = 0,
    BATTLE_POKEMON_SELECT_POKEMON_SCREEN_BUTTON_SUMMARY,
    BATTLE_POKEMON_SELECT_POKEMON_SCREEN_BUTTON_CHECK_MOVES,
    BATTLE_POKEMON_SELECT_POKEMON_SCREEN_BUTTON_CANCEL,
};

enum BattlePokemonMovesScreenButton {
    BATTLE_POKEMON_MOVES_SCREEN_BUTTON_MOVE_1 = 0,
    BATTLE_POKEMON_MOVES_SCREEN_BUTTON_MOVE_2,
    BATTLE_POKEMON_MOVES_SCREEN_BUTTON_MOVE_3,
    BATTLE_POKEMON_MOVES_SCREEN_BUTTON_MOVE_4,
    BATTLE_POKEMON_MOVES_SCREEN_BUTTON_PREV_POKEMON,
    BATTLE_POKEMON_MOVES_SCREEN_BUTTON_NEXT_POKEMON,
    BATTLE_POKEMON_MOVES_SCREEN_BUTTON_SUMMARY,
    BATTLE_POKEMON_MOVES_SCREEN_BUTTON_CANCEL,
};

enum BattleMoveSummaryScreenButton {
    BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_1 = 0,
    BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_2,
    BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_3,
    BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_4,
    BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_CANCEL,
};

enum BattleLearnMoveScreenButton {
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_1 = 0,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_2,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_3,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_4,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_TO_LEARN,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_CONTEST_STATS,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_CANCEL,
};

enum BattleRestoreMovePPScreenButton {
    BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_1 = 0,
    BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_2,
    BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_3,
    BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_4,
    BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_CANCEL,
};

enum BattlePokemonPartyScreenButton {
    BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_1 = 0,
    BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_2,
    BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_3,
    BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_4,
    BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_5,
    BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_6,
    BATTLE_POKEMON_PARTY_SCREEN_BUTTON_CANCEL,
};

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};


struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
};

struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
};

struct GridMenuCursorPosition {
    u8 xCoord1;
    u8 yCoord1;
    u8 xCoord2;
    u8 yCoord2;
    u8 upIndex;
    u8 downIndex;
    u8 leftIndex;
    u8 rightIndex;
};

enum Button {
    BUTTON_POKEMON_PARTY_SCREEN_POKEMON_1 = BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_1,
    BUTTON_POKEMON_PARTY_SCREEN_POKEMON_2 = BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_2,
    BUTTON_POKEMON_PARTY_SCREEN_POKEMON_3 = BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_3,
    BUTTON_POKEMON_PARTY_SCREEN_POKEMON_4 = BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_4,
    BUTTON_POKEMON_PARTY_SCREEN_POKEMON_5 = BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_5,
    BUTTON_POKEMON_PARTY_SCREEN_POKEMON_6 = BATTLE_POKEMON_PARTY_SCREEN_BUTTON_POKEMON_6,
    BUTTON_CANCEL = 6,
    BUTTON_SELECT_POKEMON_SCREEN_SHIFT = 7 + BATTLE_POKEMON_SELECT_POKEMON_SCREEN_BUTTON_SHIFT,
    BUTTON_SELECT_POKEMON_SCREEN_SUMMARY = 8,
    BUTTON_POKEMON_MOVES_SCREEN_SUMMARY,
    BUTTON_SELECT_POKEMON_SCREEN_CHECK_MOVES = 10,
    BUTTON_POKEMON_SUMMARY_SCREEN_CHECK_MOVES,
    BUTTON_PREV_POKEMON = 12,
    BUTTON_NEXT_POKEMON,
    BUTTON_POKEMON_MOVES_SCREEN_MOVE_1 = 14 + BATTLE_POKEMON_MOVES_SCREEN_BUTTON_MOVE_1,
    BUTTON_POKEMON_MOVES_SCREEN_MOVE_2 = 14 + BATTLE_POKEMON_MOVES_SCREEN_BUTTON_MOVE_2,
    BUTTON_POKEMON_MOVES_SCREEN_MOVE_3 = 14 + BATTLE_POKEMON_MOVES_SCREEN_BUTTON_MOVE_3,
    BUTTON_POKEMON_MOVES_SCREEN_MOVE_4 = 14 + BATTLE_POKEMON_MOVES_SCREEN_BUTTON_MOVE_4,
    BUTTON_CONTEST_STATS = 18,
    BUTTON_RESTORE_MOVE_PP_SCREEN_MOVE_1 = 19 + BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_1,
    BUTTON_RESTORE_MOVE_PP_SCREEN_MOVE_2 = 19 + BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_2,
    BUTTON_RESTORE_MOVE_PP_SCREEN_MOVE_3 = 19 + BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_3,
    BUTTON_RESTORE_MOVE_PP_SCREEN_MOVE_4 = 19 + BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_4,
    BUTTON_LEARN_MOVE_SCREEN_MOVE_1 = 23 + BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_1,
    BUTTON_LEARN_MOVE_SCREEN_MOVE_2 = 23 + BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_2,
    BUTTON_LEARN_MOVE_SCREEN_MOVE_3 = 23 + BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_3,
    BUTTON_LEARN_MOVE_SCREEN_MOVE_4 = 23 + BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_4,
    BUTTON_LEARN_MOVE_SCREEN_MOVE_TO_LEARN = 23 + BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_TO_LEARN,
    BUTTON_CONFIRM_LEARN_MOVE_SCREEN_CONFIRM = 28,
    BUTTON_CONFIRM_LEARN_MOVE_CONTEST_STATS_SCREEN_CONFIRM,
    BUTTON_MOVE_SUMMARY_SCREEN_MOVE_1 = 30 + BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_1,
    BUTTON_MOVE_SUMMARY_SCREEN_MOVE_2 = 30 + BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_2,
    BUTTON_MOVE_SUMMARY_SCREEN_MOVE_3 = 30 + BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_3,
    BUTTON_MOVE_SUMMARY_SCREEN_MOVE_4 = 30 + BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_4,
};

extern s32 ov08_022259C6;

extern s32 ov08_022259C0;

void ov08_02221F98(BattleParty *battleParty, enum Button button, enum BattleSubMenuButtonState buttonState);

void ov08_02221F98(BattleParty *battleParty, enum Button button, enum BattleSubMenuButtonState buttonState) {
    s16 offset;

    switch (button) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        offset = *(s16 *)(((u8 *)&ov08_022259C6) + (buttonState * 2));
        ManagedSprite_OffsetPositionXY(battleParty->sprites[button + 0xD], 0, offset);
        ManagedSprite_OffsetPositionXY(battleParty->sprites[button], 0, offset);
        ManagedSprite_OffsetPositionXY(battleParty->sprites[button + 7], 0, offset);
        break;
    case 7:
        offset = *(s16 *)(((u8 *)&ov08_022259C0) + (buttonState * 2));
        ManagedSprite_OffsetPositionXY(battleParty->sprites[battleParty->context->selectedPartyIndex], 0, offset);
        ManagedSprite_OffsetPositionXY(battleParty->sprites[battleParty->context->selectedPartyIndex + 7], 0, offset);
        break;
    case 14:
    case 15:
    case 16:
    case 17:
        offset = *(s16 *)(((u8 *)&ov08_022259C0) + (buttonState * 2));
        ManagedSprite_OffsetPositionXY(battleParty->sprites[button + 7], 0, offset);
        break;
    case 19:
    case 20:
    case 21:
    case 22:
        offset = *(s16 *)(((u8 *)&ov08_022259C0) + (buttonState * 2));
        ManagedSprite_OffsetPositionXY(battleParty->sprites[button + 2], 0, offset);
        break;
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
        offset = *(s16 *)(((u8 *)&ov08_022259C0) + (buttonState * 2));
        ManagedSprite_OffsetPositionXY(battleParty->sprites[button - 2], 0, offset);
        break;
    }
}
