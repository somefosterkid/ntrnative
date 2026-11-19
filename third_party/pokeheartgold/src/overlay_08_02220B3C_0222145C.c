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
#include "battle/battle.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle_party_context.h"
#include "msgdata.h"
#include "battle/battle_cursor.h"

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

int Party_GetCount(const Party *party);


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

void ov08_022213E4(ManagedSprite *sprite, u8 animationID);

u8 ov08_02221404(BattlePartyPokemon *battleParty);

void ov08_0222145C(BattleParty *battleParty);

void ov08_0222145C(BattleParty *battleParty) {
    for (u16 i = 0; i < Party_GetCount(battleParty->context->party); i++) {
        if (battleParty->partyPokemon[i].species == SPECIES_NONE) {
            continue;
        }

        u16 animationID = ov08_02221404(&battleParty->partyPokemon[i]);

        ov08_022213E4(battleParty->sprites[7 + i], animationID);
        ManagedSprite_TickNFrames(battleParty->sprites[7 + i], ((fx32)0x0000000000001000L));
    }
}

s16 ov08_022214BC(u32 statValue, s16 endCoord, s16 startCoord);

s16 ov08_022214BC(u32 statValue, s16 endCoord, s16 startCoord) {
    u32 finalCoord;

    statValue += 44;

    if (startCoord > endCoord) {
        finalCoord = ((startCoord - endCoord) * statValue) << 16;
        finalCoord = (finalCoord / 300) >> 16;

        return startCoord + (s16)finalCoord * -1;
    }

    finalCoord = ((endCoord - startCoord) * statValue) << 16;
    finalCoord = (finalCoord / 300) >> 16;

    return startCoord + (s16)finalCoord;
}
