#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "item.h"
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
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle.h"
#include "msgdata.h"
#include "battle/battle_cursor.h"
#include "battle/battle_party_context.h"

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

BOOL ov08_0221DB54(BattleParty *battleParty);

BOOL ov08_0221DB54(BattleParty *battleParty) {
    u16 move;

    if (battleParty->context->selectedMoveSlot == 4) {
        move = battleParty->context->moveToLearn;
    } else {
        move = battleParty->partyPokemon[battleParty->context->selectedPartyIndex].moves[battleParty->context->selectedMoveSlot].move;
    }

    return MoveIsHM(move);
}
