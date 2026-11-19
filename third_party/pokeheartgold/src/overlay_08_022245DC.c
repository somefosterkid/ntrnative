#include "global.h"
#include "bg_window.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "overlay_08_battle_party.h"
#include "message_printer.h"
#include "msgdata.h"
#include "battle/battle_bag_context.h"
#include "overlay_08_battle_bag.h"

u16 *ov08_022245DC(BattleBag *battleBag, u32 button, s32 buttonState, u32 unused);

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
enum BattleBagMenuScreenButton {
    BATTLE_BAG_MENU_SCREEN_BUTTON_RECOVER_HP_POCKET = 0,
    BATTLE_BAG_MENU_SCREEN_BUTTON_RECOVER_STATUS_POCKET,
    BATTLE_BAG_MENU_SCREEN_BUTTON_POKE_BALLS_POCKET,
    BATTLE_BAG_MENU_SCREEN_BUTTON_BATTLE_ITEMS_POCKET,
    BATTLE_BAG_MENU_SCREEN_BUTTON_LAST_USED_ITEM,
    BATTLE_BAG_MENU_SCREEN_BUTTON_CANCEL,
};
enum BattleBagPocketMenuScreenButton {
    BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_1 = 0,
    BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_2,
    BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_3,
    BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_4,
    BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_5,
    BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_6,
    BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_PREV_PAGE,
    BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_NEXT_PAGE,
    BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_CANCEL,
};
enum BattleBagUseItemScreenButton {
    BATTLE_BAG_USE_ITEM_SCREEN_BUTTON_USE = 0,
    BATTLE_BAG_USE_ITEM_SCREEN_BUTTON_CANCEL,
};
enum Button {
    BUTTON_MENU_SCREEN_RECOVER_HP_POCKET = BATTLE_BAG_MENU_SCREEN_BUTTON_RECOVER_HP_POCKET,
    BUTTON_MENU_SCREEN_RECOVER_STATUS_POCKET = BATTLE_BAG_MENU_SCREEN_BUTTON_RECOVER_STATUS_POCKET,
    BUTTON_MENU_SCREEN_POKE_BALLS_POCKET = BATTLE_BAG_MENU_SCREEN_BUTTON_POKE_BALLS_POCKET,
    BUTTON_MENU_SCREEN_BATTLE_ITEMS_POCKET = BATTLE_BAG_MENU_SCREEN_BUTTON_BATTLE_ITEMS_POCKET,
    BUTTON_MENU_SCREEN_LAST_USED_ITEM = BATTLE_BAG_MENU_SCREEN_BUTTON_LAST_USED_ITEM,
    BUTTON_MENU_SCREEN_CANCEL = BATTLE_BAG_MENU_SCREEN_BUTTON_CANCEL,
    BUTTON_POCKET_MENU_SCREEN_ITEM_1 = 6 + BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_1,
    BUTTON_POCKET_MENU_SCREEN_ITEM_2 = 6 + BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_2,
    BUTTON_POCKET_MENU_SCREEN_ITEM_3 = 6 + BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_3,
    BUTTON_POCKET_MENU_SCREEN_ITEM_4 = 6 + BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_4,
    BUTTON_POCKET_MENU_SCREEN_ITEM_5 = 6 + BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_5,
    BUTTON_POCKET_MENU_SCREEN_ITEM_6 = 6 + BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_6,
    BUTTON_POCKET_MENU_SCREEN_PREV_PAGE = 6 + BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_PREV_PAGE,
    BUTTON_POCKET_MENU_SCREEN_NEXT_PAGE = 6 + BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_NEXT_PAGE,
    BUTTON_POCKET_MENU_SCREEN_CANCEL = 6 + BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_CANCEL,
    BUTTON_USE_ITEM_SCREEN_USE = 15 + BATTLE_BAG_USE_ITEM_SCREEN_BUTTON_USE,
    BUTTON_USE_ITEM_SCREEN_CANCEL = 15 + BATTLE_BAG_USE_ITEM_SCREEN_BUTTON_CANCEL,
    BUTTON_POCKET_MENU_SCREEN_ALT_ITEM_1 = 17 + BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_1,
    BUTTON_POCKET_MENU_SCREEN_ALT_ITEM_2 = 17 + BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_2,
    BUTTON_POCKET_MENU_SCREEN_ALT_ITEM_3 = 17 + BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_3,
    BUTTON_POCKET_MENU_SCREEN_ALT_ITEM_4 = 17 + BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_4,
    BUTTON_POCKET_MENU_SCREEN_ALT_ITEM_5 = 17 + BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_5,
    BUTTON_POCKET_MENU_SCREEN_ALT_ITEM_6 = 17 + BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_6,
};
u16 ov08_02224684(BattleBag *battleBag, enum Button button, enum BattleSubMenuButtonState buttonState, enum BattleBagScreen screen);
void ov08_022246F4(BattleBag *battleBag, u16 *buttonData, enum Button button, enum BattleSubMenuButtonState buttonState);

u16 *ov08_022245DC(BattleBag *battleBag, u32 button, s32 buttonState, u32 unused) {
    switch (button) {
    case 0:
    case 1:
    case 2:
    case 3:
        return battleBag->menuPocketButtonData[buttonState];
    case 4:
        return battleBag->useItemButtonData[buttonState];
    case 5:
    case 14:
    case 16:
        return battleBag->cancelButtonData[buttonState];
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        return battleBag->pocketItemButtonData[buttonState];
    case 12:
        return battleBag->pocketPrevPageButtonData[buttonState];
    case 13:
        return battleBag->pocketNextPageButtonData[buttonState];
    case 15:
        return battleBag->useItemButtonData[buttonState];
    default:
        return NULL;
    }
}

u16 ov08_02224684(BattleBag *battleBag, enum Button button, enum BattleSubMenuButtonState buttonState, enum BattleBagScreen screen) {
    if (buttonState == BATTLE_SUB_MENU_BUTTON_STATE_DISABLED) {
        return 5;
    }

    switch (button) {
    case BUTTON_MENU_SCREEN_RECOVER_HP_POCKET:
    case BUTTON_MENU_SCREEN_RECOVER_STATUS_POCKET:
    case BUTTON_MENU_SCREEN_POKE_BALLS_POCKET:
    case BUTTON_MENU_SCREEN_BATTLE_ITEMS_POCKET:
        return 0;
    case BUTTON_MENU_SCREEN_LAST_USED_ITEM:
        return 3;
    case BUTTON_MENU_SCREEN_CANCEL:
    case BUTTON_POCKET_MENU_SCREEN_CANCEL:
    case BUTTON_USE_ITEM_SCREEN_CANCEL:
    case BUTTON_POCKET_MENU_SCREEN_PREV_PAGE:
    case BUTTON_POCKET_MENU_SCREEN_NEXT_PAGE:
        return 2;
    case BUTTON_POCKET_MENU_SCREEN_ITEM_1:
    case BUTTON_POCKET_MENU_SCREEN_ITEM_2:
    case BUTTON_POCKET_MENU_SCREEN_ITEM_3:
    case BUTTON_POCKET_MENU_SCREEN_ITEM_4:
    case BUTTON_POCKET_MENU_SCREEN_ITEM_5:
    case BUTTON_POCKET_MENU_SCREEN_ITEM_6:
        return 8 + battleBag->currentBattlePocket;
    case BUTTON_USE_ITEM_SCREEN_USE:
        if (screen == BATTLE_BAG_SCREEN_USE_ITEM) {
            return 8 + battleBag->currentBattlePocket;
        } else {
            return 1;
        }
    }

    return 0;
}

void ov08_022246F4(BattleBag *battleBag, u16 *buttonData, enum Button button, enum BattleSubMenuButtonState buttonState) {
    u16 *iconData;

    if (button == BUTTON_MENU_SCREEN_RECOVER_HP_POCKET) {
        iconData = battleBag->menuScreenRecoverHPPocketIconData[buttonState];
    } else if (button == BUTTON_MENU_SCREEN_RECOVER_STATUS_POCKET) {
        iconData = battleBag->menuScreenRecoverStatusPocketIconData[buttonState];
    } else if (button == BUTTON_MENU_SCREEN_POKE_BALLS_POCKET) {
        iconData = battleBag->menuScreenPokeBallsPocketIconData[buttonState];
    } else if (button == BUTTON_MENU_SCREEN_BATTLE_ITEMS_POCKET) {
        iconData = battleBag->menuScreenBattleItemsIconData[buttonState];
    } else {
        return;
    }

    for (u16 i = 0; i < 4; i++) {
        for (u16 l = 0; l < 4; l++) {
            buttonData[16 * i + ((16 - 4) / 2) + l] = iconData[4 * i + l];
        }
    }
}
