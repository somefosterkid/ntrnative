#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "list_menu_2d.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "unk_0200A090.h"
#include "overlay_82_battle_hall_app.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "pokemon_summary_app.h"
#include "message_printer.h"
#include "gf_gfx_planes.h"

struct BattleHallApp;
struct BattleHallAppSprite;
struct ListMenu2D;
typedef struct BattleHallAppSprite BattleHallAppSprite;
typedef struct ListMenu2D ListMenu2D;
void ov82_0223F5E0(BgConfig *bgConfig, u8 boxIdx, u8 background);
u8 ov82_0223F6C4(u8 cursorPos);
BOOL ov82_0223F6CC(BattleHallApp *app);
BOOL ov82_0223F6E4(BattleHallApp *app);

void ov82_0223F5E0(BgConfig *bgConfig, u8 boxIdx, u8 background) {
    u8 x, y, height, palette;

    if (background == 0) {
        palette = 0;
    } else if (background == 1) {
        palette = 5;
    } else if (background == 2) {
        palette = 4;
    } else {
        palette = 3;
    }

    u8 width = 8;
    x = (boxIdx % 4) * width;

    if (boxIdx % (2 * 4) < 4) {
        height = 5;
    } else {
        height = 4;
    }

    if (boxIdx < 4) {
        y = 0;
    } else if (boxIdx < 4 * 2) {
        y = 5;
    } else if (boxIdx < 4 * 3) {
        y = 9;
    } else if (boxIdx < 4 * 4) {
        y = 14;
    } else {
        y = 18;
    }

    BgTilemapRectChangePalette(bgConfig, GF_BG_LYR_MAIN_3, x, y, width, height, palette);

    if (background == 0) {
        palette = 0;
        width = 1;
        x = (boxIdx % 4) * 8;

        if (boxIdx % (2 * 4) < 4) {
            height = 2;
        } else {
            height = 3;
        }

        if (boxIdx < 4) {
            y = 2;
        } else if (boxIdx < (4 * 2)) {
            y = 6;
        } else if (boxIdx < (4 * 3)) {
            y = 11;
        } else if (boxIdx < (4 * 4)) {
            y = 15;
        } else {
            y = 20;
        }

        if (boxIdx < 9) {
            BgTilemapRectChangePalette(bgConfig, GF_BG_LYR_MAIN_3, x, y, width, height, 1);
        } else {
            BgTilemapRectChangePalette(bgConfig, GF_BG_LYR_MAIN_3, x, y, width, height, 2);
        }
    }
}

u8 ov82_0223F6C4(u8 cursorPos) {
    if (cursorPos >= 17) {
        return 17;
    }

    return cursorPos;
}

BOOL ov82_0223F6CC(BattleHallApp *app) {
    if (app->challengeType == 0) {
        if (app->currentStreak == 50 || app->currentStreak == 170) {
            return 1;
        }
    }

    return 0;
}

BOOL ov82_0223F6E4(BattleHallApp *app) {
    return app->matronBattleAvailable;
}
