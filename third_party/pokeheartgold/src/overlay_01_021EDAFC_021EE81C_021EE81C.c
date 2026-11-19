#include "global.h"

#include "constants/maps.h"
#include "constants/sndseq.h"

#include "pokeathlon/pokeathlon_save.h"

#include "coins.h"
#include "error_handling.h"
#include "font.h"
#include "frontier_data.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "list_menu_2d.h"
#include "overlay_01.h"
#include "overlay_03.h"
#include "player_data.h"
#include "pm_string.h"
#include "render_window.h"
#include "save_pokegear.h"
#include "screen_fade.h"
#include "string_util.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"

int MapNumToFloorNo(u32 mapId) {
    switch ((s32)mapId) {
    case 189:
        return 0;
    case 190:
        return 1;
    case 115:
        return 0;
    case 225:
        return 1;
    case 402:
        return 0;
    case 403:
        return 1;
    case 191:
        return 1;
    case 192:
        return 2;
    case 193:
        return 3;
    case 194:
        return 4;
    case 195:
        return 5;
    case 196:
        return 6;
    case 200:
        return 0;
    case 370:
        return 0;
    case 371:
        return 1;
    case 372:
        return 2;
    case 373:
        return 3;
    case 374:
        return 4;
    case 375:
        return 5;
    case 376:
        return 0;
    case 377:
        return 1;
    case 378:
        return 2;
    case 379:
        return 3;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        return 0;
    }
}
