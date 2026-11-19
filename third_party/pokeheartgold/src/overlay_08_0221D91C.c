#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "message_printer.h"
#include "palette.h"
#include "pm_string.h"
#include "seal_case.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02020A0C.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "pokemon_types_def.h"
#include "trainer_data.h"
#include "battle/battle_party_context.h"
#include "overlay_08_battle_party.h"
#include "msgdata.h"
#include "pokemon.h"
#include "message_format.h"
#include "battle/battle.h"

struct BattleSystem;


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


typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;

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

BOOL ov08_0221DB24(BattleParty *battleParty, u8 partySlot);

Trainer *BattleSystem_GetTrainer(BattleSystem *battleSys, int battler);

int BattleSystem_GetBattlerIdPartner(BattleSystem *battleSys, int battler);

u8 ov08_0221DAC4(BattleParty *battleParty);

BOOL ov08_0221D91C(BattleParty *battleParty);

BOOL ov08_0221D91C(BattleParty *battleParty) {
    BattlePartyPokemon *pokemon = &battleParty->partyPokemon[battleParty->context->selectedPartyIndex];

    if (ov08_0221DB24(battleParty, battleParty->context->selectedPartyIndex) == 1) {
        String *string = NewString_ReadMsgData(battleParty->messageLoader, 80);

        int trainerSlot = BattleSystem_GetBattlerIdPartner(battleParty->context->battleSys, battleParty->context->battler);
        BufferTrainerNameFromDataStruct(battleParty->stringTemplate, 0, BattleSystem_GetTrainer(battleParty->context->battleSys, trainerSlot));
        StringExpandPlaceholders(battleParty->stringTemplate, battleParty->string, string);
        String_Delete(string);
        return 0;
    }

    if (pokemon->curHP == 0) {
        String *string = NewString_ReadMsgData(battleParty->messageLoader, 77);
        BufferBoxMonNickname(battleParty->stringTemplate, 0, Mon_GetBoxMon(pokemon->mon));
        StringExpandPlaceholders(battleParty->stringTemplate, battleParty->string, string);
        String_Delete(string);
        return 0;
    }

    if (battleParty->context->pokemonPartySlots[battleParty->context->selectedPartyIndex] == battleParty->context->playerPokemonPartySlot || battleParty->context->pokemonPartySlots[battleParty->context->selectedPartyIndex] == battleParty->context->partnerPokemonPartySlot) {
        String *string = NewString_ReadMsgData(battleParty->messageLoader, 76);
        BufferBoxMonNickname(battleParty->stringTemplate, 0, Mon_GetBoxMon(pokemon->mon));
        StringExpandPlaceholders(battleParty->stringTemplate, battleParty->string, string);
        String_Delete(string);
        return 0;
    }

    if (ov08_0221DAC4(battleParty) == 1) {
        ReadMsgDataIntoString(battleParty->messageLoader, 79, battleParty->string);
        return 0;
    }

    if (battleParty->context->doubleBattleFirstSelectedPartySlot != 6 && battleParty->context->pokemonPartySlots[battleParty->context->selectedPartyIndex] == battleParty->context->doubleBattleFirstSelectedPartySlot) {
        pokemon = &battleParty->partyPokemon[battleParty->context->selectedPartyIndex];
        String *string = NewString_ReadMsgData(battleParty->messageLoader, 93);

        BufferBoxMonNickname(battleParty->stringTemplate, 0, Mon_GetBoxMon(pokemon->mon));
        StringExpandPlaceholders(battleParty->stringTemplate, battleParty->string, string);
        String_Delete(string);
        return 0;
    }

    if (battleParty->context->moveToLearn != MOVE_NONE) {
        pokemon = &battleParty->partyPokemon[battleParty->partySlotLearningMove];
        String *string = NewString_ReadMsgData(battleParty->messageLoader, 78);

        BufferBoxMonNickname(battleParty->stringTemplate, 0, Mon_GetBoxMon(pokemon->mon));
        StringExpandPlaceholders(battleParty->stringTemplate, battleParty->string, string);
        String_Delete(string);
        return 0;
    }

    return 1;
}

u8 ov08_0221DAC4(BattleParty *battleParty) {
    return battleParty->partyPokemon[battleParty->context->selectedPartyIndex].isEgg != FALSE;
}
