#include "global.h"
#include "overlay_82_battle_hall_app.h"
#include "pm_string.h"
#include "save.h"
#include "options.h"
#include "filesystem.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "bg_window.h"
#include "pokemon_summary_app.h"
#include "message_format.h"
#include "overlay_manager.h"
#include "message_printer.h"
#include "msgdata.h"
#include "list_menu_items.h"
#include "gf_gfx_planes.h"
#include "overlay_82.h"

struct BattleHallApp;

struct BattleHallAppSprite;


struct G2dRenderer;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct ListMenuItem;

struct Window;

u8 sub_02030BD0(u8 idx, u8 *typeRanks);

typedef struct BattleHallAppSprite BattleHallAppSprite;

u8 ov80_02237920(u8 cursorPos);

typedef struct ListMenu2DTemplate {
    ListMenuItem *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
} ListMenu2DTemplate;

typedef struct ListMenu2D ListMenu2D;

enum BattleHallAppState {
    STATE_FADE_IN = 0,
    STATE_MON_SUMMARY,
    STATE_SELECT_BATTLE,
    STATE_MULTI_CONFIRM,
    STATE_SYNC_BEFORE_EXIT,
    STATE_FADE_OUT,
    STATE_CLOSE_MESSAGE,
};


u8 ov82_0223F6C4(u8 cursorPos);

void LoadMainScreenBackground(BattleHallApp *app, enum GFBgLayer bgLayer);

void LoadSubScreenBackground(BattleHallApp *app, enum GFBgLayer bgLayer);

void BattleArcadeGameBoard_SetState(BattleHallApp *app, int *state, enum BattleHallAppState nextState);

void ov82_0223F134(BattleHallApp *app, Window *window);

void ov82_0223F134(BattleHallApp *app, Window *window) {
    for (int r = 0; r < 5; r++) {
        for (int c = 0; c < 4; c++) {
            u8 type = ov80_02237920(r * 4 + c);

            if (type != 0xfe && type != TYPE_MYSTERY) {
                u8 v3 = r * 4 + c;
                u8 rank = sub_02030BD0(ov82_0223F6C4(v3), app->typeRanks) + 1;

                if (rank > 10) {
                    rank = 10;
                }

                ov82_0223F10C(app, window, rank, 18 + (64 * c), 4 + (36 * r));
            }
        }
    }
}
