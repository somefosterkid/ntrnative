#include "global.h"

#include "struct_defs/resdat_narc_structs.h"

#include "battle/battle_system.h"

#include "gf_3d_loader.h"
#include "message_printer.h"
#include "unk_0200A090.h"
#include "unk_02020A0C.h"
#include "unk_02077678.h"
#include "filesystem.h"
#include "party_menu.h"
#include "sprite_system.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle_party_context.h"
#include "msgdata.h"
#include "battle/battle_cursor.h"

enum MoveClass {
    CLASS_PHYSICAL = 0,
    CLASS_SPECIAL = 1,
    CLASS_STATUS = 2,
};


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

void ov08_02220800(BattleParty *battleParty);

void ov08_02220878(BattleParty *battleParty);
void ov08_02220928(BattleParty *battleParty);

void ov08_02220800(BattleParty *battleParty) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(battleParty->context->battleSys);

    SpriteSystem_LoadMoveTypeAndCategoryIconsPlttToBuffer(battleParty->palette, PLTTBUF_SUB_OBJ, spriteSystem, battleParty->spriteManager, NNS_G2D_VRAM_TYPE_2DSUB, 45065);
    SpriteSystem_LoadMoveTypeAndCategoryIconsCellAndAnim(spriteSystem, battleParty->spriteManager, 45065, 45065);

    for (u32 i = 45070; i <= 45072 + 4; i++) {
        SpriteSystem_LoadMoveTypeIconCharResObj(spriteSystem, battleParty->spriteManager, NNS_G2D_VRAM_TYPE_2DSUB, TYPE_NORMAL, i);
    }

    SpriteSystem_LoadMoveCategoryIconCharResObj(spriteSystem, battleParty->spriteManager, NNS_G2D_VRAM_TYPE_2DSUB, CLASS_PHYSICAL, 45077);
}

void ov08_02220878(BattleParty *battleParty) {
    NARC *narc;
    SpriteSystem *spriteSystem;

    narc = NARC_New(NARC_graphic_plist_gra, battleParty->context->heapID);
    spriteSystem = BattleSystem_GetSpriteSystem(battleParty->context->battleSys);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(battleParty->palette, PLTTBUF_SUB_OBJ, spriteSystem, battleParty->spriteManager, narc, (s32)sub_0207CAA0(), 0, 1, 2, 0xB00A);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, battleParty->spriteManager, narc, (s32)sub_0207CAA4(), 0, 0xB00A);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, battleParty->spriteManager, narc, (s32)sub_0207CAA8(), 0, 0xB00A);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, battleParty->spriteManager, narc, (s32)sub_0207CA9C(), 0, 2, 0xB016);
    NARC_Delete(narc);
}

void ov08_02220928(BattleParty *battleParty) {
    SpriteSystem *spriteSystem;

    spriteSystem = BattleSystem_GetSpriteSystem(battleParty->context->battleSys);
    SpriteSystem_LoadPaletteBuffer(battleParty->palette, PLTTBUF_SUB_OBJ, spriteSystem, battleParty->spriteManager, NARC_a_0_7_1, 0x1B, 0, 1, 2, 0xB00B);
    SpriteSystem_LoadCellResObj(spriteSystem, battleParty->spriteManager, NARC_a_0_7_1, 0x19, 0, 0xB00B);
    SpriteSystem_LoadAnimResObj(spriteSystem, battleParty->spriteManager, NARC_a_0_7_1, 0x18, 0, 0xB00B);
    SpriteSystem_LoadCharResObj(spriteSystem, battleParty->spriteManager, NARC_a_0_7_1, 0x1A, 0, 2, 0xB017);
}
