#include "global.h"

#include "constants/heap.h"

#include "battle/battle.h"
#include "battle/battle_bag_context.h"
#include "battle/battle_cursor.h"
#include "battle/battle_finger.h"
#include "battle/battle_system.h"

#include "bag_cursor.h"
#include "bag_types_def.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02020A0C.h"
#include "wild_encounter.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_bag.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle_party_context.h"

typedef struct BagItem BagItem;

typedef u16 charcode_t;

struct BagItem {
    u16 item;
    u16 quantity;
};

enum BattleBagScreen {
    BATTLE_BAG_SCREEN_MENU = 0,
    BATTLE_BAG_SCREEN_POCKET_MENU,
    BATTLE_BAG_SCREEN_USE_ITEM,
};

void ov08_02223000(BattleParty *battleParty);

void ov08_022230F4(void *arg0);

void ov08_022231E8(void *arg0);

void ov08_022233B8(BattleBag *battleBag);

void ov08_02223D08(void *arg0);

void ov08_02224134(BattleBag *battleBag, enum BattleBagScreen screen);

void ov08_0222421C(BattleBag *battleBag, enum BattleBagScreen screen);

s32 ov08_02223480(BattleBag *, s32);

s32 ov08_02223BF4(BattleBag *);

s32 ov08_02223F94(BattleBag *, u8);

s32 ov08_02224A50(BattleBag *, s32);

BattleSubMenuCursor *ov08_02224B64(enum HeapID);

s32 ov08_02224B90(BattleSubMenuCursor *, s32);

u8 ov08_0222276C(BattleBag *battleBag);

u8 ov08_0222276C(BattleBag *battleBag) {
    *(s16 *)REG_DB_BLDCNT_ADDR = 0;
    battleBag->cursor = ov08_02224B64(battleBag->context->heapID);
    ov08_02223000((BattleParty *)battleBag);
    ov08_022230F4(battleBag);
    ov08_022231E8(battleBag);
    FontID_Alloc(FONT_UNK_4, battleBag->context->heapID);
    battleBag->currentBattlePocket = (u8)BagCursor_Battle_GetPocket(BattleSystem_GetBagCursor(battleBag->context->battleSys));
    ov08_02223BF4(battleBag);
    ov08_02224A50(battleBag, battleBag->currentScreen);
    ov08_022233B8(battleBag);
    ov08_02223480(battleBag, battleBag->currentScreen);
    ov08_02223D08(battleBag);
    ov08_02223F94(battleBag, battleBag->currentScreen);
    if (battleBag->context->isCursorEnabled != 0) {
        ov08_02224B90(battleBag->cursor, 1);
    }
    ov08_02224134(battleBag, (enum BattleBagScreen)battleBag->currentScreen);
    ov08_0222421C(battleBag, (enum BattleBagScreen)battleBag->currentScreen);
    PaletteData_BeginPaletteFade(battleBag->palette, 0xAU, 0xFFFFU, -8, 0x10U, 0U, 0U);
    if (battleBag->context->isInCatchTutorial == 1) {
        return 0xCU;
    }
    return 1U;
}
