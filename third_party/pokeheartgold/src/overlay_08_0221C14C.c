#include "global.h"

#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "gf_3d_loader.h"
#include "item.h"
#include "message_printer.h"
#include "palette.h"
#include "pm_string.h"
#include "seal_case.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02020A0C.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle.h"
#include "battle/battle_party_context.h"

struct BattleSystem;

struct BattleCursor;

typedef u16 charcode_t;

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

typedef struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
} StringTemplateArgHeader;

typedef struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
} StringTemplateArg;

enum BattlePartyMode {
    BATTLE_PARTY_MODE_SELECT_POKEMON = 0,
    BATTLE_PARTY_MODE_SELECT_POKEMON_NO_CANCEL,
    BATTLE_PARTY_MODE_USE_ITEM,
    BATTLE_PARTY_MODE_LEARN_MOVE,
};

void ov08_022220AC(BattleParty *battleParty, u8 button);

static u8 ov08_0221C1C8(BattleParty *battleParty);

BOOL ov08_0221D438(BattleParty *battleParty);

u8 ov08_0221C14C(BattleParty *battleParty);

u8 ov08_0221C14C(BattleParty *battleParty) {
    if (PaletteData_GetSelectedBuffersBitmask(battleParty->palette) != 0) {
        return 1;
    }

    if (ov08_0221D438(battleParty) == 1) {
        if (battleParty->context->selectedPartyIndex == 6) {
            if (battleParty->context->battlePartyMode != BATTLE_PARTY_MODE_SELECT_POKEMON_NO_CANCEL) {
                PlaySE(1501);
                ov08_022220AC(battleParty, 6);
                return 25;
            }
        } else {
            PlaySE(1501);
            ov08_022220AC(battleParty, battleParty->context->selectedPartyIndex);

            if (battleParty->context->battlePartyMode == BATTLE_PARTY_MODE_USE_ITEM) {
                return ov08_0221C1C8(battleParty);
            } else {
                battleParty->queuedState = 7;
            }

            return 22;
        }
    }

    return 1;
}


u32 GetMonData(Pokemon *mon, int attr, void *ptr);

u8 ov08_0221D5DC(BattleParty *battleParty, s32 partySlot);

enum ItemDataParam {
    ITEM_PARAM_PRICE,
    ITEM_PARAM_HOLD_EFFECT,
    ITEM_PARAM_EFFECT_PARAM,
    ITEM_PARAM_PREVENT_TOSS,
    ITEM_PARAM_CAN_REGISTER,
    ITEM_PARAM_FIELD_POCKET,
    ITEM_PARAM_FIELD_USE_FUNC,
    ITEM_PARAM_BATTLE_USE_CATEGORY,
    ITEM_PARAM_PLUCK_EFFECT,
    ITEM_PARAM_FLING_EFFECT,
    ITEM_PARAM_FLING_POWER,
    ITEM_PARAM_NATURAL_GIFT_POWER,
    ITEM_PARAM_NATURAL_GIFT_TYPE,
    ITEM_PARAM_BATTLE_POCKET,
    ITEM_PARAM_PARTY_USE,
    ITEM_PARAM_HEAL_SLEEP,
    ITEM_PARAM_HEAL_POISON,
    ITEM_PARAM_HEAL_BURN,
    ITEM_PARAM_HEAL_FREEZE,
    ITEM_PARAM_HEAL_PARALYSIS,
    ITEM_PARAM_HEAL_CONFUSION,
    ITEM_PARAM_HEAL_ATTRACT,
    ITEM_PARAM_GUARD_SPEC,
    ITEM_PARAM_REVIVE,
    ITEM_PARAM_REVIVE_ALL,
    ITEM_PARAM_LEVEL_UP,
    ITEM_PARAM_EVOLVE,
    ITEM_PARAM_ATK_STAGES,
    ITEM_PARAM_DEF_STAGES,
    ITEM_PARAM_SPATK_STAGES,
    ITEM_PARAM_SPDEF_STAGES,
    ITEM_PARAM_SPEED_STAGES,
    ITEM_PARAM_ACC_STAGES,
    ITEM_PARAM_CRIT_STAGES,
    ITEM_PARAM_PP_UP,
    ITEM_PARAM_PP_MAX,
    ITEM_PARAM_PP_RESTORE,
    ITEM_PARAM_PP_RESTORE_ALL,
    ITEM_PARAM_HP_RESTORE,
    ITEM_PARAM_GIVE_HP_EVS,
    ITEM_PARAM_GIVE_ATK_EVS,
    ITEM_PARAM_GIVE_DEF_EVS,
    ITEM_PARAM_GIVE_SPEED_EVS,
    ITEM_PARAM_GIVE_SPATK_EVS,
    ITEM_PARAM_GIVE_SPDEF_EVS,
    ITEM_PARAM_GIVE_FRIENDSHIP_LOW,
    ITEM_PARAM_GIVE_FRIENDSHIP_MED,
    ITEM_PARAM_GIVE_FRIENDSHIP_HIGH,
    ITEM_PARAM_HP_EVS,
    ITEM_PARAM_ATK_EVS,
    ITEM_PARAM_DEF_EVS,
    ITEM_PARAM_SPEED_EVS,
    ITEM_PARAM_SPATK_EVS,
    ITEM_PARAM_SPDEF_EVS,
    ITEM_PARAM_HP_RESTORED,
    ITEM_PARAM_PP_RESTORED,
    ITEM_PARAM_FRIENDSHIP_LOW,
    ITEM_PARAM_FRIENDSHIP_MED,
    ITEM_PARAM_FRIENDSHIP_HIGH,
};

Pokemon *BattleSystem_GetPartyMon(BattleSystem *battleSys, int battler, int slot);

BOOL BattleSystem_RecoverStatus(BattleSystem *battleSys, int battler, int partySlot, int moveSlot, int item);

void ov08_022201C0(BattleParty *battleParty);

void ov08_0222057C(BattleParty *battleParty);

enum BattlePartyUseRestorationItemState {
    BATTLE_PARTY_USE_RESTORATION_ITEM_STATE_INITIALISING = 0,
    BATTLE_PARTY_USE_RESTORATION_ITEM_STATE_RESTORING_HP,
    BATTLE_PARTY_USE_RESTORATION_ITEM_STATE_RESTORING_PP,
    BATTLE_PARTY_USE_RESTORATION_ITEM_STATE_FINISHING,
    BATTLE_PARTY_USE_RESTORATION_ITEM_STATE_START_RESTORING_HP,
};

void ov08_0221DBCC(BattleSystem *battleSys, u16 item, u16 category, enum HeapID heapID);

static u8 ov08_0221C1C8(BattleParty *battleParty) {
    BattlePartyContext *context = battleParty->context;

    if (context->selectedPartyIndex == 0 && context->embargoRemainingTurns[0] != 0 || context->selectedPartyIndex == 1 && context->embargoRemainingTurns[1] != 0) {
        ov08_0222057C(battleParty);
        ov08_022201C0(battleParty);
        battleParty->context->selectedPartyIndex = 6;
        battleParty->queuedState = 25;
        return 17;
    }

    if (GetItemAttr(context->selectedBattleBagItem, ITEM_PARAM_PP_RESTORE, context->heapID) != 0
        && GetItemAttr(context->selectedBattleBagItem, ITEM_PARAM_PP_RESTORE_ALL, context->heapID) == 0
        && battleParty->partyPokemon[context->selectedPartyIndex].isEgg == 0) {
        battleParty->queuedState = 13;
        return 22;
    }

    if (BattleSystem_RecoverStatus(context->battleSys, context->battler, context->pokemonPartySlots[context->selectedPartyIndex], 0, context->selectedBattleBagItem) == 1) {
        if (GetItemAttr(context->selectedBattleBagItem, ITEM_PARAM_PP_RESTORE_ALL, context->heapID) != 0) {
            battleParty->queuedState = 13;
        } else {
            if (ov08_0221D5DC(battleParty, context->selectedPartyIndex) == 1 && GetItemAttr(context->selectedBattleBagItem, ITEM_PARAM_REVIVE, context->heapID) == 0) {
                ov08_0221DBCC(context->battleSys, context->selectedBattleBagItem, context->selectedBattleBagPocket, context->heapID);
                battleParty->partyPokemon[context->selectedPartyIndex].mon = BattleSystem_GetPartyMon(context->battleSys, context->battler, context->pokemonPartySlots[context->selectedPartyIndex]);
                context->currentDamage = GetMonData(battleParty->partyPokemon[context->selectedPartyIndex].mon, MON_DATA_HP, 0L);
                context->currentDamage -= battleParty->partyPokemon[context->selectedPartyIndex].curHP;
                battleParty->queuedState = 25;
            } else {
                battleParty->queuedState = 23;
            }
        }

        battleParty->useItemState = BATTLE_PARTY_USE_RESTORATION_ITEM_STATE_INITIALISING;
        return 22;
    } else {
        ReadMsgDataIntoString(battleParty->messageLoader, 81, battleParty->string);
        ov08_022201C0(battleParty);
        battleParty->context->selectedPartyIndex = 6;
        battleParty->queuedState = 25;
        return 17;
    }

    return 17;
}

enum BattleSelectPokemonScreenButton {
    BATTLE_POKEMON_SELECT_POKEMON_SCREEN_BUTTON_SHIFT = 0,
    BATTLE_POKEMON_SELECT_POKEMON_SCREEN_BUTTON_SUMMARY,
    BATTLE_POKEMON_SELECT_POKEMON_SCREEN_BUTTON_CHECK_MOVES,
    BATTLE_POKEMON_SELECT_POKEMON_SCREEN_BUTTON_CANCEL,
};

/* DS: the callers were built against the wider return type below; the PC takes the definition's (abilint RET) */
#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov08_0221D4B0(BattleParty *battleParty);
#else
enum BattleSelectPokemonScreenButton ov08_0221D4B0(BattleParty *battleParty);
#endif

BOOL ov08_0221D91C(BattleParty *battleParty);

u8 ov08_0221DAC4(BattleParty *battleParty);

u8 ov08_0221C318(BattleParty *battleParty);

u8 ov08_0221C318(BattleParty *battleParty) {
    enum BattleSelectPokemonScreenButton selectPokemonScreenButtonPressed = ov08_0221D4B0(battleParty);

    switch (selectPokemonScreenButtonPressed) {
    case BATTLE_POKEMON_SELECT_POKEMON_SCREEN_BUTTON_SHIFT:
        PlaySE(1501);
        ov08_022220AC(battleParty, 7 + BATTLE_POKEMON_SELECT_POKEMON_SCREEN_BUTTON_SHIFT);

        if (ov08_0221D91C(battleParty) == 1) {
            return 25;
        }

        battleParty->queuedState = 15;
        return 22;
    case BATTLE_POKEMON_SELECT_POKEMON_SCREEN_BUTTON_SUMMARY:
        if (ov08_0221DAC4(battleParty) == 1) {
            break;
        }

        PlaySE(1501);
        ov08_022220AC(battleParty, 7 + BATTLE_POKEMON_SELECT_POKEMON_SCREEN_BUTTON_SUMMARY);
        battleParty->queuedState = 8;
        return 22;
    case BATTLE_POKEMON_SELECT_POKEMON_SCREEN_BUTTON_CHECK_MOVES:
        if (ov08_0221DAC4(battleParty) == 1) {
            break;
        }

        PlaySE(1501);
        ov08_022220AC(battleParty, 10);
        battleParty->queuedState = 9;
        return 22;
    case BATTLE_POKEMON_SELECT_POKEMON_SCREEN_BUTTON_CANCEL:
        PlaySE(1501);
        ov08_022220AC(battleParty, 6);
        battleParty->queuedState = 6;
        return 22;
    }

    return 2;
}

enum BattlePokemonSummaryScreenButton {
    BATTLE_POKEMON_SUMMARY_SCREEN_BUTTON_PREV_POKEMON = 0,
    BATTLE_POKEMON_SUMMARY_SCREEN_BUTTON_NEXT_POKEMON,
    BATTLE_POKEMON_SUMMARY_SCREEN_BUTTON_CHECK_MOVES,
    BATTLE_POKEMON_SUMMARY_SCREEN_BUTTON_CANCEL,
};

#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov08_0221D4F8(BattleParty *battleParty);
#else
enum BattlePokemonSummaryScreenButton ov08_0221D4F8(BattleParty *battleParty);
#endif

u8 ov08_0221D614(BattleParty *battleParty, s32 selectedPartyIndex, s32 indexChange);

u8 ov08_0221C3C8(BattleParty *battleParty);

u8 ov08_0221C3C8(BattleParty *battleParty) {
    enum BattlePokemonSummaryScreenButton pokemonSummaryScreenButtonPressed = ov08_0221D4F8(battleParty);

    switch (pokemonSummaryScreenButtonPressed) {
    case BATTLE_POKEMON_SUMMARY_SCREEN_BUTTON_PREV_POKEMON: {
        u8 newPartyIndex = ov08_0221D614(battleParty, battleParty->context->selectedPartyIndex, -1);

        if (newPartyIndex == 255) {
            break;
        }

        battleParty->context->selectedPartyIndex = newPartyIndex;
    }
        PlaySE(1501);
        ov08_022220AC(battleParty, 12);
        battleParty->queuedState = 14;
        return 22;
    case BATTLE_POKEMON_SUMMARY_SCREEN_BUTTON_NEXT_POKEMON: {
        u8 newPartyIndex = ov08_0221D614(battleParty, battleParty->context->selectedPartyIndex, 1);

        if (newPartyIndex == 255) {
            break;
        }

        battleParty->context->selectedPartyIndex = newPartyIndex;
    }
        PlaySE(1501);
        ov08_022220AC(battleParty, 12 + 1);
        battleParty->queuedState = 14;
        return 22;
    case BATTLE_POKEMON_SUMMARY_SCREEN_BUTTON_CHECK_MOVES:
        PlaySE(1501);
        ov08_022220AC(battleParty, 10 + 1);
        battleParty->queuedState = 9;
        return 22;
    case BATTLE_POKEMON_SUMMARY_SCREEN_BUTTON_CANCEL:
        PlaySE(1501);
        ov08_022220AC(battleParty, 6);
        battleParty->selectPokemonPreviousScreenButton = BATTLE_POKEMON_SELECT_POKEMON_SCREEN_BUTTON_SUMMARY;
        battleParty->queuedState = 7;
        return 22;
    }

    return 3;
}


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

#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov08_0221D540(BattleParty *battleParty);
#else
enum BattlePokemonMovesScreenButton ov08_0221D540(BattleParty *battleParty);
#endif

u8 ov08_0221C488(BattleParty *battleParty);

u8 ov08_0221C488(BattleParty *battleParty) {
    enum BattlePokemonMovesScreenButton pokemonMovesScreenButtonPressed = ov08_0221D540(battleParty);

    switch (pokemonMovesScreenButtonPressed) {
    case BATTLE_POKEMON_MOVES_SCREEN_BUTTON_MOVE_1:
    case BATTLE_POKEMON_MOVES_SCREEN_BUTTON_MOVE_2:
    case BATTLE_POKEMON_MOVES_SCREEN_BUTTON_MOVE_3:
    case BATTLE_POKEMON_MOVES_SCREEN_BUTTON_MOVE_4:
        if (battleParty->partyPokemon[battleParty->context->selectedPartyIndex].moves[pokemonMovesScreenButtonPressed].move == MOVE_NONE) {
            break;
        }

        PlaySE(1501);
        ov08_022220AC(battleParty, 14 + pokemonMovesScreenButtonPressed);
        battleParty->context->selectedMoveSlot = pokemonMovesScreenButtonPressed;
        battleParty->queuedState = 10;
        return 22;
    case BATTLE_POKEMON_MOVES_SCREEN_BUTTON_PREV_POKEMON: {
        u8 newPartyIndex = ov08_0221D614(battleParty, battleParty->context->selectedPartyIndex, -1);

        if (newPartyIndex == 255) {
            break;
        }

        battleParty->context->selectedPartyIndex = newPartyIndex;
    }
        PlaySE(1501);
        ov08_022220AC(battleParty, 12);
        battleParty->queuedState = 14;
        return 22;

    case BATTLE_POKEMON_MOVES_SCREEN_BUTTON_NEXT_POKEMON: {
        u8 newPartyIndex = ov08_0221D614(battleParty, battleParty->context->selectedPartyIndex, 1);

        if (newPartyIndex == 255) {
            break;
        }

        battleParty->context->selectedPartyIndex = newPartyIndex;
    }
        PlaySE(1501);
        ov08_022220AC(battleParty, 12 + 1);
        battleParty->queuedState = 14;
        return 22;
    case BATTLE_POKEMON_MOVES_SCREEN_BUTTON_SUMMARY:
        PlaySE(1501);
        ov08_022220AC(battleParty, 8 + 1);
        battleParty->queuedState = 8;
        return 22;
    case BATTLE_POKEMON_MOVES_SCREEN_BUTTON_CANCEL:
        PlaySE(1501);
        ov08_022220AC(battleParty, 6);
        battleParty->selectPokemonPreviousScreenButton = BATTLE_POKEMON_SELECT_POKEMON_SCREEN_BUTTON_CHECK_MOVES;
        battleParty->queuedState = 7;
        return 22;
    }

    return 4;
}

enum BattleMoveSummaryScreenButton {
    BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_1 = 0,
    BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_2,
    BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_3,
    BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_4,
    BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_CANCEL,
};

#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov08_0221D588(BattleParty *battleParty);
#else
enum BattleMoveSummaryScreenButton ov08_0221D588(BattleParty *battleParty);
#endif

u8 ov08_0221C58C(BattleParty *battleParty);

u8 ov08_0221C58C(BattleParty *battleParty) {
    enum BattleMoveSummaryScreenButton moveSummaryScreenButtonPressed = ov08_0221D588(battleParty);

    switch (moveSummaryScreenButtonPressed) {
    case BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_1:
    case BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_2:
    case BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_3:
    case BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_MOVE_4:
        if (battleParty->context->selectedMoveSlot != moveSummaryScreenButtonPressed && battleParty->partyPokemon[battleParty->context->selectedPartyIndex].moves[moveSummaryScreenButtonPressed].move == MOVE_NONE) {
            break;
        }

        PlaySE(1501);
        battleParty->context->selectedMoveSlot = moveSummaryScreenButtonPressed;
        return 10;
    case BATTLE_MOVE_SUMMARY_SCREEN_BUTTON_CANCEL:
        PlaySE(1501);
        ov08_022220AC(battleParty, 6);
        battleParty->queuedState = 9;
        return 22;
    }

    return 5;
}
