#include "global.h"

#include "constants/heap.h"

#include "battle/battle.h"

#include "bg_window.h"
#include "filesystem.h"
#include "move.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "wild_encounter.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "msgdata.h"
#include "battle/battle_cursor.h"
#include "battle/battle_party_context.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;


typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct GridMenuCursorPosition GridMenuCursorPosition;

typedef u16 charcode_t;

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};


struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
};

struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
};

struct GridMenuCursorPosition {
    u8 xCoord1;
    u8 yCoord1;
    u8 xCoord2;
    u8 yCoord2;
    u8 upIndex;
    u8 downIndex;
    u8 leftIndex;
    u8 rightIndex;
};

s32 ov08_02220A8C(ManagedSprite *, s32, s32);

void ov08_02220B3C(BattleParty *battleParty, ManagedSprite *sprite, int moveClass);

s32 ov08_02220BB4(BattleParty *, void *, s32 *);

s32 ov08_02220C08(u16, ManagedSprite *, s32, s32);

s32 ov08_02220C3C(u8, ManagedSprite *, s32, s32);

extern s32 ov08_022253B4;

void ov08_02221088(BattleParty *battleParty);

void ov08_02221088(BattleParty *battleParty) {
    BattlePartyContext *context;
    u8 partyIndex;
    u8 moveSlot;
    void *pokemon;

    partyIndex = battleParty->context->selectedPartyIndex;
    pokemon = battleParty->partyPokemon + partyIndex;
    ov08_02220A8C(battleParty->sprites[partyIndex + 7], 0x18, 0xC);
    ov08_02220BB4(battleParty, pokemon, &ov08_022253B4);
    ov08_02220A8C(battleParty->sprites[battleParty->context->selectedMoveSlot + 0x15], 0x88, 0x30);
    ov08_02220C08(*(u16 *)((u8 *)pokemon + 0x1A), battleParty->sprites[battleParty->context->selectedPartyIndex], 0x20, 0x14);
    ov08_02220C3C(*(u8 *)((u8 *)pokemon + 0x2D), battleParty->sprites[battleParty->context->selectedPartyIndex + 0x20], 0x28, 0x14);
    context = battleParty->context;
    moveSlot = context->selectedMoveSlot;
    if (moveSlot < 4) {
        ov08_02220B3C(battleParty, battleParty->sprites[0x1A], *(u8 *)((u8 *)pokemon + moveSlot * 8 + 0x35));
    } else {
        ov08_02220B3C(battleParty, battleParty->sprites[0x1A], GetMoveAttr(context->moveToLearn, MOVEATTR_CLASS));
    }
    ov08_02220A8C(battleParty->sprites[0x1A], 0x18, 0x58);
}
