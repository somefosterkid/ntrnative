#include "global.h"

#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "font.h"
#include "gf_3d_loader.h"
#include "message_printer.h"
#include "palette.h"
#include "party.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02020A0C.h"
#include "unk_02088288.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle.h"
#include "msgdata.h"
#include "battle/battle_cursor.h"
#include "battle/battle_party_context.h"

struct BattleSystem;


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

void ov08_0221F550(BattleParty *battleParty, u8 windowIndex);

void ov08_0221F5B0(BattleParty *battleParty, u8 partyIndex);

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

void ov08_0221DDCC(BattleParty *battleParty, u32 windowIndex, enum Font font, u16 partyIndex, u8 xOffset, u8 yOffset);

void ov08_0221E340(BattleParty *battleParty, u32 textID);

void ov08_0221F4A4(BattleParty *battleParty);

void ov08_0221F4A4(BattleParty *battleParty) {
    for (s32 i = 0; i < Party_GetCount(battleParty->context->party); i++) {
        FillWindowPixelBuffer(&battleParty->windows[i], 0);

        if (battleParty->partyPokemon[i].species == SPECIES_NONE) {
            continue;
        }

        ov08_0221DDCC(battleParty, i, (enum Font)4, i, 32, 7);

        if (battleParty->partyPokemon[i].isEgg == 0) {
            ov08_0221F550(battleParty, i);
        }

        if (Pokemon_GetStatusIconId(battleParty->partyPokemon[i].mon) != SUMMARY_CONDITION_NONE) {
            continue;
        }

        ov08_0221F5B0(battleParty, i);
    }

    if (battleParty->context->battlePartyMode == BATTLE_PARTY_MODE_USE_ITEM) {
        ov08_0221E340(battleParty, 7);
    } else {
        ov08_0221E340(battleParty, 6);
    }
}
