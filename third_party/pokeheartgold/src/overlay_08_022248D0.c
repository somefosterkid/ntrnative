#include "global.h"

#include "sprite_system.h"
#include "bg_window.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "overlay_08_battle_bag.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "overlay_08_battle_party.h"
#include "message_printer.h"
#include "msgdata.h"
#include "battle/battle_bag_context.h"

void ov08_022248D0(BattleBag *arg0, u32 arg1, s32 arg2);

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
void ov08_02224938(BattleBag *battleBag, u8 pressedButton, u8 unused);

void ov08_022248D0(BattleBag *arg0, u32 arg1, s32 arg2) {
    s32 v0;
    ManagedSprite *managedSprite;
    u8 v1;

    v0 = arg2;
    if ((arg1 >= 6U) && (arg1 <= 0xBU)) {
        s32 idx = arg1 - 6;

        managedSprite = arg0->sprites[idx];
        goto block_7;
    }
    if (arg1 == 4) {
        v1 = 0;
loop_5:
        managedSprite = arg0->sprites[v1];
        if (ManagedSprite_GetDrawFlag(managedSprite) == 0) {
            v1 += 1;
            if ((u32)v1 < 6U) {
                goto loop_5;
            }
        }
block_7:
        switch (v0) {
        case 0:
        case 2:
            ManagedSprite_OffsetPositionXY(managedSprite, 0, 2);
            return;
        case 1:
            ManagedSprite_OffsetPositionXY(managedSprite, 0, -4);
            break;
        }
    }
}

void ov08_02224938(BattleBag *battleBag, u8 pressedButton, u8 unused) {
    battleBag->pressedButtonState = BATTLE_SUB_MENU_BUTTON_STATE_UNPRESSED;
    battleBag->unused2 = 0;
    battleBag->pressedButton = pressedButton;
    battleBag->unused3 = unused;
    battleBag->isAButtonPressed = 1;
}
