#include "global.h"

#include "struct_defs/resdat_narc_structs.h"

#include "battle/battle.h"
#include "battle/battle_cursor.h"
#include "battle/battle_finger.h"

#include "bag_types_def.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_3d_loader.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02020A0C.h"
#include "overlay_08_battle_bag.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_bag_context.h"

typedef struct BagItem {
    u16 item;
    u16 quantity;
} BagItem;

typedef u16 charcode_t;

enum BattleBagScreen {
    BATTLE_BAG_SCREEN_MENU = 0,
    BATTLE_BAG_SCREEN_POCKET_MENU,
    BATTLE_BAG_SCREEN_USE_ITEM,
};

extern const WindowTemplate ov08_02225BB8[5];

extern const WindowTemplate ov08_02225C10[26];

extern const WindowTemplate ov08_02225B98[4];

void ov08_022233DC(BattleBag *battleBag, enum BattleBagScreen screen);

void ov08_022233DC(BattleBag *battleBag, enum BattleBagScreen screen) {
    const WindowTemplate *windowTemplates;

    switch (screen) {
    case BATTLE_BAG_SCREEN_MENU:
        windowTemplates = ov08_02225BB8;
        battleBag->numWindows = 5;
        break;
    case BATTLE_BAG_SCREEN_POCKET_MENU:
        windowTemplates = ov08_02225C10;
        battleBag->numWindows = 26;
        break;
    case BATTLE_BAG_SCREEN_USE_ITEM:
        windowTemplates = ov08_02225B98;
        battleBag->numWindows = 4;
        break;
    }

    battleBag->windows = AllocWindows(battleBag->context->heapID, battleBag->numWindows);

    for (u8 i = 0; i < battleBag->numWindows; i++) {
        AddWindow(battleBag->background, &battleBag->windows[i], &windowTemplates[i]);
    }
}
