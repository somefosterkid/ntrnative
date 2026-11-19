#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "font_types_def.h"
#include "gf_3d_loader.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "render_text.h"
#include "render_window.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "text.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02020A0C.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle.h"
#include "battle/battle_cursor.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_bag_context.h"
#include "overlay_08_battle_bag.h"

struct BattleBagContext;
struct BattleSubMenuCursor;
struct BattleSystem;
struct Indicator;
typedef struct Indicator Indicator;
typedef struct BagItem {
    u16 item;
    u16 quantity;
} BagItem;
static void ov08_02223B48(BattleBag *battleBag);
struct Indicator {
    ManagedSprite *sprite;
    SysTask *animTask;
    int x;
    int y;
    int bounceAngle;
    fx32 subscreenOffset;
    s16 exitTimer;
    u8 exitPending;
    u8 hasDropped;
    u8 isExiting;
    u8 exitStep;
    u8 exitFrameCount;
};
typedef u16 charcode_t;
u8 BattleSystem_GetTextFrameDelay(BattleSystem *battleSys);
enum ItemBattleCategory {
    ITEM_BATTLE_CATEGORY_RECOVER_HP = 0,
    ITEM_BATTLE_CATEGORY_RECOVER_STATUS = 1,
    ITEM_BATTLE_CATEGORY_POKE_BALLS = 2,
    ITEM_BATTLE_CATEGORY_BATTLE_ITEMS = 3,
    ITEM_BATTLE_CATEGORY_MAX = 4,
};
BOOL Bag_HasItem(Bag *bag, u16 item, u16 count, enum HeapID heapID);

void ov08_02223B20(BattleBag *battleBag);
static void ov08_02223B48(BattleBag *battleBag);
BOOL ov08_02223B78(BattleBag *battleBag);
void ov08_02223BA8(BattleBag *battleBag);

void ov08_02223B20(BattleBag *battleBag) {
    DrawFrameAndWindow2(&battleBag->messageBoxWindow, 1, 994, 14);
    FillWindowPixelBuffer(&battleBag->messageBoxWindow, 15);
    ov08_02223B48(battleBag);
}

static void ov08_02223B48(BattleBag *battleBag) {
    TextFlags_SetCanABSpeedUpPrint(1);
    battleBag->textPrinterID = AddTextPrinterParameterized(&battleBag->messageBoxWindow, FONT_MESSAGE, battleBag->string, 0, 0, BattleSystem_GetTextFrameDelay(battleBag->context->battleSys), 0L);
}

BOOL ov08_02223B78(BattleBag *battleBag) {
    if (battleBag->context->lastUsedItem == ITEM_NONE) {
        return 0;
    }

    if (Bag_HasItem(battleBag->context->bag, battleBag->context->lastUsedItem, 1, battleBag->context->heapID) == 0) {
        battleBag->context->lastUsedItem = ITEM_NONE;
        battleBag->context->lastUsedItemPocket = ITEM_BATTLE_CATEGORY_RECOVER_HP;
        return 0;
    }

    return 1;
}

void ov08_02223BA8(BattleBag *battleBag) {
    for (u32 i = 0; i < 36; i++) {
        if (battleBag->context->lastUsedItem == battleBag->items[battleBag->currentBattlePocket][i].id) {
            battleBag->context->pocketCurrentPagePositions[battleBag->currentBattlePocket] = i % 6;
            battleBag->context->pocketCurrentPages[battleBag->currentBattlePocket] = i / 6;
            break;
        }
    }
}
