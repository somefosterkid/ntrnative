#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_08_battle_bag.h"
#include "gf_gfx_planes.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "msgdata.h"
#include "message_printer.h"
#include "battle/battle_bag_context.h"

struct BattleBagContext;
struct MessagePrinter;
struct Indicator;
typedef struct Indicator Indicator;
void ov08_0222370C(BattleBag *battleBag, u32 slot_index);

void ov08_0222377C(BattleBag *battleBag);

void ov08_0222377C(BattleBag *battleBag) {
    FillBgTilemapRect(battleBag->background, GF_BG_LYR_SUB_1, 0, 0, 0, 32, 19, 17);

    for (u16 i = 0; i < 6; i++) {
        ov08_0222370C(battleBag, i);
    }

    battleBag->useAltPocketMenuWindows ^= 1;
}
