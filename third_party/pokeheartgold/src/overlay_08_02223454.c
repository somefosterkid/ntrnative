#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "message_printer.h"
#include "msgdata.h"
#include "battle/battle_bag_context.h"
#include "overlay_08_battle_bag.h"

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

void ov08_02223454(BattleBag *battleBag);
void ov08_02223464(BattleBag *battleBag);

void ov08_02223454(BattleBag *battleBag) {
    WindowArray_Delete(battleBag->windows, battleBag->numWindows);
}

void ov08_02223464(BattleBag *battleBag) {
    WindowArray_Delete(battleBag->windows, battleBag->numWindows);
    RemoveWindow(&battleBag->messageBoxWindow);
}
