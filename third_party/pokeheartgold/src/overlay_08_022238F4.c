#include "global.h"

#include "bg_window.h"
#include "font.h"
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
enum ItemBattleCategory {
    ITEM_BATTLE_CATEGORY_RECOVER_HP = 0,
    ITEM_BATTLE_CATEGORY_RECOVER_STATUS = 1,
    ITEM_BATTLE_CATEGORY_POKE_BALLS = 2,
    ITEM_BATTLE_CATEGORY_BATTLE_ITEMS = 3,
    ITEM_BATTLE_CATEGORY_MAX = 4,
};
typedef struct BagItem {
    u16 item;
    u16 quantity;
} BagItem;
typedef u32 TextColor;
void ov08_022234A4(BattleBag *battleBag, u8 windowIndex, u32 textID, enum Font font, u32 yOffset, TextColor color);

void ov08_022238F4(BattleBag *battleBag);

void ov08_022238F4(BattleBag *battleBag) {
    FillWindowPixelBuffer(&battleBag->windows[BATTLE_BAG_POCKET_MENU_WINDOW_POCKET_NAME], 0);

    switch (battleBag->currentBattlePocket) {
    case ITEM_BATTLE_CATEGORY_RECOVER_HP:
        ov08_022234A4(battleBag, BATTLE_BAG_POCKET_MENU_WINDOW_POCKET_NAME, 22, FONT_SYSTEM, 4, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))));
        ov08_022234A4(battleBag, BATTLE_BAG_POCKET_MENU_WINDOW_POCKET_NAME, 23, FONT_SYSTEM, 20, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))));
        break;
    case ITEM_BATTLE_CATEGORY_RECOVER_STATUS:
        ov08_022234A4(battleBag, BATTLE_BAG_POCKET_MENU_WINDOW_POCKET_NAME, 24, FONT_SYSTEM, 4, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))));
        ov08_022234A4(battleBag, BATTLE_BAG_POCKET_MENU_WINDOW_POCKET_NAME, 25, FONT_SYSTEM, 20, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))));
        break;
    case ITEM_BATTLE_CATEGORY_POKE_BALLS:
        ov08_022234A4(battleBag, BATTLE_BAG_POCKET_MENU_WINDOW_POCKET_NAME, 26, FONT_SYSTEM, 12, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))));
        break;
    case ITEM_BATTLE_CATEGORY_BATTLE_ITEMS:
        ov08_022234A4(battleBag, BATTLE_BAG_POCKET_MENU_WINDOW_POCKET_NAME, 27, FONT_SYSTEM, 12, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))));
        break;
    }
}
