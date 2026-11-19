#include "global.h"

#include "constants/heap.h"

#include "battle/battle.h"
#include "battle/battle_cursor.h"
#include "battle/battle_finger.h"

#include "bag_types_def.h"
#include "bg_window.h"
#include "filesystem.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02020A0C.h"
#include "overlay_08_battle_bag.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_bag_context.h"

void ov08_022239B4(BattleBag *battleBag);

void ov08_02223AA0(BattleBag *battleBag);

void ov08_022234FC(BattleBag *battleBag);

void ov08_02223480(BattleBag *arg0, s32 arg1);

void ov08_02223480(BattleBag *arg0, s32 arg1) {
    switch (arg1) {
    case 0:
        ov08_022234FC(arg0);
        return;
    case 1:
        ov08_022239B4(arg0);
        return;
    case 2:
        ov08_02223AA0(arg0);
        return;
    }
}
