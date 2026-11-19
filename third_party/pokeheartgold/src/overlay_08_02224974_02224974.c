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

void ov08_022247E4(BattleBag *battleBag, u8 button, u8 buttonState, u8 screen);

void ov08_02224858(BattleBag *battleBag, u8 button, enum BattleSubMenuButtonState buttonState);

void ov08_022248D0(BattleBag *battleBag, u32 button, enum BattleSubMenuButtonState buttonState);

void ov08_02224974(BattleBag *battleBag);

void ov08_02224974(BattleBag *battleBag) {
    if (battleBag->isAButtonPressed == 0) {
        return;
    }

    switch (battleBag->pressedButtonState) {
    case BATTLE_SUB_MENU_BUTTON_STATE_UNPRESSED:
        ov08_022247E4(battleBag, battleBag->pressedButton, BATTLE_SUB_MENU_BUTTON_STATE_PRESSING, battleBag->currentScreen);
        ov08_02224858(battleBag, battleBag->pressedButton, BATTLE_SUB_MENU_BUTTON_STATE_PRESSING);
        ov08_022248D0(battleBag, battleBag->pressedButton, BATTLE_SUB_MENU_BUTTON_STATE_PRESSING);
        battleBag->unused2 = 0;
        battleBag->pressedButtonState = BATTLE_SUB_MENU_BUTTON_STATE_PRESSING;
        break;
    case BATTLE_SUB_MENU_BUTTON_STATE_PRESSING:
        ov08_022247E4(battleBag, battleBag->pressedButton, BATTLE_SUB_MENU_BUTTON_STATE_PRESSED, battleBag->currentScreen);
        ov08_02224858(battleBag, battleBag->pressedButton, BATTLE_SUB_MENU_BUTTON_STATE_PRESSED);
        ov08_022248D0(battleBag, battleBag->pressedButton, BATTLE_SUB_MENU_BUTTON_STATE_PRESSED);
        battleBag->unused2 = 0;
        battleBag->pressedButtonState = BATTLE_SUB_MENU_BUTTON_STATE_PRESSED;
        break;
    case BATTLE_SUB_MENU_BUTTON_STATE_PRESSED:
        ov08_022247E4(battleBag, battleBag->pressedButton, BATTLE_SUB_MENU_BUTTON_STATE_UNPRESSED, battleBag->currentScreen);
        ov08_02224858(battleBag, battleBag->pressedButton, BATTLE_SUB_MENU_BUTTON_STATE_UNPRESSED);
        ov08_022248D0(battleBag, battleBag->pressedButton, BATTLE_SUB_MENU_BUTTON_STATE_UNPRESSED);
        battleBag->unused2 = 0;
        battleBag->pressedButtonState = BATTLE_SUB_MENU_BUTTON_STATE_UNPRESSED;
        battleBag->isAButtonPressed = 0;
        break;
    }
}
