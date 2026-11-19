#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "struct_defs/resdat_narc_structs.h"
#include "filesystem.h"
#include "font_types_def.h"
#include "gf_3d_loader.h"
#include "message_printer.h"
#include "palette.h"
#include "pokemon_types_def.h"
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
#include "battle/battle_cursor.h"

void ov08_0221E120(BattleParty *battleParty, s32 windowIndex, s32 partyIndex);

struct BattleSystem;
typedef u32 TextColor;
void ov08_0221E1A8(BattleParty *battleParty, u32 windowIndex, u32 partyIndex);

void ov08_0221E120(BattleParty *battleParty, s32 windowIndex, s32 partyIndex) {
    String *dest;
    String *src;

    dest = String_New(0x10, battleParty->context->heapID);
    src = NewString_ReadMsgData(battleParty->messageLoader, 8);
    BufferAbilityName(battleParty->stringTemplate, 0, battleParty->partyPokemon[partyIndex].ability);
    StringExpandPlaceholders(battleParty->stringTemplate, dest, src);
    AddTextPrinterParameterizedWithColor(&battleParty->windows[windowIndex], FONT_SYSTEM, dest, 0, 0, 0xff, 0xf0e00, NULL);
    String_Delete(src);
    String_Delete(dest);
    ScheduleWindowCopyToVram(&battleParty->windows[windowIndex]);
}

void ov08_0221E1A8(BattleParty *battleParty, u32 windowIndex, u32 partyIndex) {
    BattlePartyPokemon *pokemon = &battleParty->partyPokemon[partyIndex];
    String *formattedString;

    if (pokemon->heldItem == ITEM_NONE) {
        formattedString = NewString_ReadMsgData(battleParty->messageLoader, 20);
    } else {
        formattedString = String_New(18, battleParty->context->heapID);
        String *string = NewString_ReadMsgData(battleParty->messageLoader, 9);

        BufferItemName(battleParty->stringTemplate, 0, pokemon->heldItem);
        StringExpandPlaceholders(battleParty->stringTemplate, formattedString, string);
        String_Delete(string);
    }

    AddTextPrinterParameterizedWithColor(&battleParty->windows[windowIndex], FONT_SYSTEM, formattedString, 0, 0, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(formattedString);
    ScheduleWindowCopyToVram(&battleParty->windows[windowIndex]);
}
