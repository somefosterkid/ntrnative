#include "global.h"

#include "battle/battle_system.h"

#include "pokemon.h"
#include "sprite_system.h"
#include "unk_02005D10.h"
#include "unk_02088288.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle_party_context.h"

struct BattleSubMenuCursor;

struct MessagePrinter;

struct MsgData;

struct MessageFormat;

enum BattlePartyScreen {
    BATTLE_PARTY_SCREEN_POKEMON_PARTY = 0,
    BATTLE_PARTY_SCREEN_SELECT_POKEMON,
    BATTLE_PARTY_SCREEN_POKEMON_SUMMARY,
    BATTLE_PARTY_SCREEN_POKEMON_MOVES,
    BATTLE_PARTY_SCREEN_MOVE_SUMMARY,
    BATTLE_PARTY_SCREEN_RESTORE_MOVE_PP,
    BATTLE_PARTY_SCREEN_LEARN_MOVE,
    BATTLE_PARTY_SCREEN_CONFIRM_LEARN_MOVE,
    BATTLE_PARTY_SCREEN_LEARN_MOVE_CONTEST_STATS,
    BATTLE_PARTY_SCREEN_CONFIRM_LEARN_MOVE_CONTEST_STATS,
};

enum SummaryStatus {
    SUMMARY_CONDITION_POKERUS = 0,
    SUMMARY_CONDITION_PARALYSIS,
    SUMMARY_CONDITION_FREEZE,
    SUMMARY_CONDITION_SLEEP,
    SUMMARY_CONDITION_POISON,
    SUMMARY_CONDITION_BURN,
    SUMMARY_CONDITION_FAINTED,
    SUMMARY_CONDITION_NONE,
};

void ov08_022225A4(BattleParty *battleParty);

void ov08_0221F550(BattleParty *battleParty, u8 windowIndex);

void ov08_0221F5B0(BattleParty *battleParty, u8 partyIndex);

void ov08_02220064(BattleParty *battleParty, u16 windowIndex, u16 moveIndex);

void ov08_022201C0(BattleParty *battleParty);

void ov08_02220224(BattleParty *battleParty);

enum BattlePartyUseRestorationItemState {
    BATTLE_PARTY_USE_RESTORATION_ITEM_STATE_INITIALISING = 0,
    BATTLE_PARTY_USE_RESTORATION_ITEM_STATE_RESTORING_HP,
    BATTLE_PARTY_USE_RESTORATION_ITEM_STATE_RESTORING_PP,
    BATTLE_PARTY_USE_RESTORATION_ITEM_STATE_FINISHING,
    BATTLE_PARTY_USE_RESTORATION_ITEM_STATE_START_RESTORING_HP,
};

void ov08_0221DBCC(BattleSystem *battleSys, u16 item, u16 category, enum HeapID heapID);

u8 ov08_0221CA90(BattleParty *battleParty);

u8 ov08_0221CA90(BattleParty *battleParty) {
    BattlePartyContext *context = battleParty->context;

    switch (battleParty->useItemState) {
    case BATTLE_PARTY_USE_RESTORATION_ITEM_STATE_INITIALISING:
        battleParty->partyPokemon[context->selectedPartyIndex].mon = BattleSystem_GetPartyMon(context->battleSys, context->battler, context->pokemonPartySlots[context->selectedPartyIndex]);
        ov08_02220224(battleParty);

        if (battleParty->currentScreen == BATTLE_PARTY_SCREEN_RESTORE_MOVE_PP) {
            battleParty->selectedPokemonCurrentMovePPs[0] = (u16)GetMonData(battleParty->partyPokemon[context->selectedPartyIndex].mon, MON_DATA_MOVE1_PP + context->selectedMoveSlot, 0L);
            battleParty->useItemState = BATTLE_PARTY_USE_RESTORATION_ITEM_STATE_RESTORING_PP;
        } else {
            battleParty->partyPokemon[context->selectedPartyIndex].summaryStatus = Pokemon_GetStatusIconId(battleParty->partyPokemon[context->selectedPartyIndex].mon);

            if (battleParty->partyPokemon[context->selectedPartyIndex].summaryStatus == SUMMARY_CONDITION_NONE) {
                ManagedSprite_SetDrawFlag(battleParty->sprites[13 + context->selectedPartyIndex], 0);
                ov08_0221F5B0(battleParty, context->selectedPartyIndex);
            }

            battleParty->selectedPokemonCurrentHP = GetMonData(battleParty->partyPokemon[context->selectedPartyIndex].mon, MON_DATA_HP, 0L);
            battleParty->useItemState = BATTLE_PARTY_USE_RESTORATION_ITEM_STATE_START_RESTORING_HP;
        }

        PlaySE(1516);
        break;
    case BATTLE_PARTY_USE_RESTORATION_ITEM_STATE_RESTORING_HP:
        if (battleParty->partyPokemon[context->selectedPartyIndex].curHP != battleParty->selectedPokemonCurrentHP) {
            battleParty->partyPokemon[context->selectedPartyIndex].curHP++;
            ov08_0221F550(battleParty, context->selectedPartyIndex);
            break;
        }

        battleParty->useItemState = BATTLE_PARTY_USE_RESTORATION_ITEM_STATE_FINISHING;
        break;
    case BATTLE_PARTY_USE_RESTORATION_ITEM_STATE_RESTORING_PP:
        if (battleParty->partyPokemon[context->selectedPartyIndex].moves[context->selectedMoveSlot].currentPP != battleParty->selectedPokemonCurrentMovePPs[0]) {
            battleParty->partyPokemon[context->selectedPartyIndex].moves[context->selectedMoveSlot].currentPP++;
            ov08_02220064(battleParty, 1 + context->selectedMoveSlot, context->selectedMoveSlot);
            break;
        }

        battleParty->useItemState = BATTLE_PARTY_USE_RESTORATION_ITEM_STATE_FINISHING;
        break;
    case BATTLE_PARTY_USE_RESTORATION_ITEM_STATE_FINISHING:
        ov08_0221DBCC(context->battleSys, context->selectedBattleBagItem, context->selectedBattleBagPocket, context->heapID);
        ov08_022201C0(battleParty);
        battleParty->queuedState = 25;
        return 17;
    case BATTLE_PARTY_USE_RESTORATION_ITEM_STATE_START_RESTORING_HP:
        if (battleParty->partyPokemon[context->selectedPartyIndex].curHP != battleParty->selectedPokemonCurrentHP) {
            battleParty->partyPokemon[context->selectedPartyIndex].curHP++;
            ov08_0221F550(battleParty, context->selectedPartyIndex);
            ov08_022225A4(battleParty);
        }

        battleParty->useItemState = BATTLE_PARTY_USE_RESTORATION_ITEM_STATE_RESTORING_HP;
        break;
    }

    return 23;
}

enum BattlePartyUseAllMovePPRestorationItemState {
    BATTLE_PARTY_USE_ALL_MOVE_PP_RESTORATION_ITEM_STATE_INTIALISING = 0,
    BATTLE_PARTY_USE_ALL_MOVE_PP_RESTORATION_ITEM_STATE_RESTORING_PP,
    BATTLE_PARTY_USE_ALL_MOVE_PP_RESTORATION_ITEM_STATE_FINISHING,
};

u8 ov08_0221CC38(BattleParty *battleParty);

u8 ov08_0221CC38(BattleParty *battleParty) {
    BattlePartyContext *context = battleParty->context;

    switch (battleParty->useItemState) {
    case BATTLE_PARTY_USE_ALL_MOVE_PP_RESTORATION_ITEM_STATE_INTIALISING:
        battleParty->partyPokemon[context->selectedPartyIndex].mon = BattleSystem_GetPartyMon(context->battleSys, context->battler, context->pokemonPartySlots[context->selectedPartyIndex]);

        for (u32 i = 0; i < 4; i++) {
            if (battleParty->partyPokemon[context->selectedPartyIndex].moves[i].move == MOVE_NONE) {
                continue;
            }

            battleParty->selectedPokemonCurrentMovePPs[i] = (u16)GetMonData(battleParty->partyPokemon[context->selectedPartyIndex].mon, MON_DATA_MOVE1_PP + i, 0L);
        }

        ov08_02220224(battleParty);
        PlaySE(1516);
        battleParty->useItemState = BATTLE_PARTY_USE_ALL_MOVE_PP_RESTORATION_ITEM_STATE_RESTORING_PP;
        break;
    case BATTLE_PARTY_USE_ALL_MOVE_PP_RESTORATION_ITEM_STATE_RESTORING_PP: {
        u32 restoredMoves = 0;

        for (u32 i = 0; i < 4; i++) {
            if (battleParty->partyPokemon[context->selectedPartyIndex].moves[i].move == MOVE_NONE) {
                restoredMoves++;
                continue;
            }

            if (battleParty->partyPokemon[context->selectedPartyIndex].moves[i].currentPP != battleParty->selectedPokemonCurrentMovePPs[i]) {
                battleParty->partyPokemon[context->selectedPartyIndex].moves[i].currentPP++;
                ov08_02220064(battleParty, 1 + i, i);
            } else {
                restoredMoves++;
            }
        }

        if (restoredMoves == 4) {
            battleParty->useItemState = BATTLE_PARTY_USE_ALL_MOVE_PP_RESTORATION_ITEM_STATE_FINISHING;
        }
    } break;
    case BATTLE_PARTY_USE_ALL_MOVE_PP_RESTORATION_ITEM_STATE_FINISHING:
        ov08_0221DBCC(context->battleSys, context->selectedBattleBagItem, context->selectedBattleBagPocket, context->heapID);
        ov08_022201C0(battleParty);
        battleParty->queuedState = 25;
        return 17;
    }

    return 24;
}
