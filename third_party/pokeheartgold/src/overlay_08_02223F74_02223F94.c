#include "global.h"

#include "bg_window.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_08_battle_bag.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "overlay_08_battle_party.h"
#include "message_printer.h"
#include "msgdata.h"
#include "battle/battle_bag_context.h"

struct BattleBagContext;

struct BattleSubMenuCursor;

struct MessagePrinter;

struct Indicator;

struct MsgData;

struct MessageFormat;

typedef struct Indicator Indicator;

typedef struct BagItem {
    u16 item;
    u16 quantity;
} BagItem;

enum BattleBagScreen {
    BATTLE_BAG_SCREEN_MENU = 0,
    BATTLE_BAG_SCREEN_POCKET_MENU,
    BATTLE_BAG_SCREEN_USE_ITEM,
};

static void ov08_02223FD8(void *arg0);

void ov08_0222400C(BattleBag *battleBag);

void ov08_02224064(BattleBag *battleBag);

void ov08_02223F94(BattleBag *battleBag, enum BattleBagScreen screen);

void ov08_02223F94(BattleBag *battleBag, enum BattleBagScreen screen) {
    for (u32 i = 0; i < 6; i++) {
        ManagedSprite_SetDrawFlag(battleBag->sprites[i], 0);
    }

    switch (screen) {
    case BATTLE_BAG_SCREEN_MENU:
        ov08_02223FD8(battleBag);
        break;
    case BATTLE_BAG_SCREEN_POCKET_MENU:
        ov08_0222400C(battleBag);
        break;
    case BATTLE_BAG_SCREEN_USE_ITEM:
        ov08_02224064(battleBag);
        break;
    }
}

s32 ov08_02223E3C(void *, u16, s32);

s32 ov08_02223E74(void *, u16, s16, s32);

s32 ov08_02223F74(ManagedSprite *, s32, s32);

static void ov08_02223FD8(void *arg0) {
    u16 v0;

    v0 = ((BattleBag *)arg0)->context->lastUsedItem;
    if (v0 != 0) {
        ov08_02223E3C(arg0, v0, 0xB4B7);
        ov08_02223E74(arg0, ((BattleBag *)arg0)->context->lastUsedItem, 0, 0xB4B7);
        ov08_02223F74(*((void **)&((BattleBag *)arg0)->sprites[0]), 0x18, 0xB2);
    }
}
