#include "global.h"
#include "overlay_08_battle_party.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "bg_window.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "message_printer.h"
#include "msgdata.h"
#include "battle/battle_party_context.h"

struct BattleParty;

struct BattlePartyContext;

struct BattlePartyPokemon;

struct BattlePartyPokemonMove;

struct BattleSubMenuCursor;

struct BgConfig;

struct MessagePrinter;

struct ManagedSprite;

struct MsgData;

struct PaletteData;

struct Pokemon;

struct SpriteManager;

struct String;

struct MessageFormat;

struct Window;

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

u8 ov08_0221D5DC(BattleParty *battleParty, s32 partySlot);

BOOL ov08_0221DB04(BattleParty *battleParty);

void ov08_0221D6CC(BattleParty *battleParty, enum BattlePartyScreen screen);

void ChangeBattlePartyScreen(BattleParty *battleParty, enum BattlePartyScreen screen);

void ov08_0221D8B0(BattleParty *battleParty, enum BattlePartyScreen screen);

u8 ov08_0221D614(BattleParty *battleParty, s32 selectedPartyIndex, s32 indexChange);

u8 ov08_0221D614(BattleParty *battleParty, s32 selectedPartyIndex, s32 indexChange) {
    s32 prevPartyIndex = selectedPartyIndex;

    if (ov08_0221DB04(battleParty) == 1) {
        u8 partyIndexes2v2[] = { 0, 2, 4, 1, 3, 5 };

        for (selectedPartyIndex = 0; selectedPartyIndex < 6; selectedPartyIndex++) {
            if (prevPartyIndex == partyIndexes2v2[selectedPartyIndex]) {
                break;
            }
        }

        while (1) {
            selectedPartyIndex += indexChange;

            if (selectedPartyIndex < 0) {
                selectedPartyIndex = 5;
            } else if (selectedPartyIndex >= 6) {
                selectedPartyIndex = 0;
            }

            if (prevPartyIndex == partyIndexes2v2[selectedPartyIndex]) {
                break;
            }

            if (ov08_0221D5DC(battleParty, partyIndexes2v2[selectedPartyIndex]) != 0) {
                if (battleParty->partyPokemon[partyIndexes2v2[selectedPartyIndex]].isEgg == 0) {
                    return partyIndexes2v2[selectedPartyIndex];
                }
            }
        }
    } else {
        while (1) {
            selectedPartyIndex += indexChange;

            if (selectedPartyIndex < 0) {
                selectedPartyIndex = 5;
            } else if (selectedPartyIndex >= 6) {
                selectedPartyIndex = 0;
            }

            if (prevPartyIndex == selectedPartyIndex) {
                break;
            }

            if (ov08_0221D5DC(battleParty, selectedPartyIndex) != 0) {
                if (battleParty->partyPokemon[selectedPartyIndex].isEgg == 0) {
                    return (u8)selectedPartyIndex;
                }
            }
        }
    }

    return 255;
}
