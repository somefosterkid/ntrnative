#include "global.h"

#include "struct_defs/resdat_narc_structs.h"

#include "battle/battle_system.h"

#include "gf_3d_loader.h"
#include "message_printer.h"
#include "sprite_system.h"
#include "unk_0200A090.h"
#include "unk_02020A0C.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle_party_context.h"
#include "msgdata.h"
#include "battle/battle_cursor.h"

ManagedSprite *ov08_022209B8(void *, u32);

s32 ov08_02220AAC(void *);

void ov08_02220A28(BattleParty *battleParty);

void ov08_02220A28(BattleParty *battleParty) {
    u32 i = 0;

    do {
        battleParty->sprites[i] = ov08_022209B8(battleParty, i);
        i++;
    } while (i < 38);
    ov08_02220AAC(battleParty);
}


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

void ov08_02221698(BattleParty *battleParty);

void ov08_02220A50(BattleParty *battleParty);

void ov08_02220A50(BattleParty *battleParty) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(battleParty->context->battleSys);

    for (u32 i = 0; i < 38; i++) {
        Sprite_DeleteAndFreeResources(battleParty->sprites[i]);
    }

    ov08_02221698(battleParty);
    SpriteSystem_FreeResourcesAndManager(spriteSystem, battleParty->spriteManager);
}
