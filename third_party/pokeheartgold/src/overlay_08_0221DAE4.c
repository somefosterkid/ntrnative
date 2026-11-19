#include "global.h"

#include "struct_defs/resdat_narc_structs.h"

#include "battle/battle_system.h"

#include "gf_3d_loader.h"
#include "message_printer.h"
#include "unk_0200A090.h"
#include "unk_02020A0C.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "msgdata.h"
#include "battle/battle_cursor.h"
#include "battle/battle_party_context.h"


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

BOOL ov08_0221DAE4(BattleParty *battleParty);

BOOL ov08_0221DAE4(BattleParty *battleParty) {
    u32 battleType = BattleSystem_GetBattleType(battleParty->context->battleSys);

    if (battleType != ((1 << 1) | (1 << 3) | (1 << 6))
        && battleType != (((1 << 1) | (1 << 0)) | (1 << 3) | (1 << 6))
        && battleType & ((1 << 1) | (1 << 4))) {
        return 1;
    }

    return 0;
}

BOOL ov08_0221DB04(BattleParty *battleParty);

BOOL ov08_0221DB04(BattleParty *battleParty) {
    u32 battleType = BattleSystem_GetBattleType(battleParty->context->battleSys);

    if (battleType != ((1 << 1) | (1 << 3) | (1 << 6))
        && battleType != (((1 << 1) | (1 << 0)) | (1 << 3) | (1 << 6))
        && battleType & (1 << 3)) {
        return 1;
    }

    return 0;
}
