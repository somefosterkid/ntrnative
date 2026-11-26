#include "global.h"

#include "constants/heap.h"

#include "battle_castle_app_sprite_manager.h"
#include "screen_fade.h"
#include "overlay_83_battle_castle_self_app.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "save.h"
#include "options.h"
#include "filesystem.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "message_format.h"
#include "battle_castle_save.h"
#include "save_frontier.h"
#include "battle_castle_app_sprite.h"
#include "msgdata.h"
#include "list_menu_items.h"
#include "list_menu.h"
#include "message_printer.h"
#include "struct_defs/resdat_narc_structs.h"
#include "gf_gfx_planes.h"

struct BattleCastleSelfApp;

struct ListMenuCursor;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct ListMenuItem;

struct Window;

enum TextAlignment {
    TEXT_ALIGN_LEFT,
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_RIGHT
};

typedef struct ListMenuCursor ListMenuCursor;

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

BOOL ov80_02237D8C(u8 challengeType);

void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID);

BOOL IsPaletteFadeFinished(void);

void sub_02037AC0(u8 syncNo);

BOOL sub_02037B38(u8 syncState);

void sub_02037BEC(void);

enum BattleCastleAppState {
    STATE_FADE_IN = 0,
    STATE_MAIN_FLOW,
    STATE_SYNC_PURCHASE,
    STATE_SYNC_BEFORE_EXIT,
    STATE_FADE_OUT
};

void ov83_0223E10C(BattleCastleSelfApp *app);

BOOL ov83_02241368(BattleCastleSelfApp *app, u16 cmd, u16 slot);

void LoadMainBackground(BattleCastleSelfApp *app, enum GFBgLayer bgLayer);

void LoadSummaryBackground(BattleCastleSelfApp *app, enum GFBgLayer bgLayer);

void LoadMovesListBackground(BattleCastleSelfApp *app, enum GFBgLayer bgLayer);

void LoadItemSelectBackground(BattleCastleSelfApp *app, enum GFBgLayer bgLayer);

void LoadSubScreenBackground(BattleCastleSelfApp *app, enum GFBgLayer bgLayer);


u8 PrintMessage(BattleCastleSelfApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font, enum TextAlignment alignment);

void PrintStaticMessage(BattleCastleSelfApp *app, Window *window, int entryID, u16 xOffset, u16 yOffset, enum TextAlignment alignment);

void ChangeState(BattleCastleSelfApp *app, int *state, enum BattleCastleAppState nextState);

BOOL ov83_0223E008(BattleCastleSelfApp *app);

BOOL ov83_0223E008(BattleCastleSelfApp *app) {
    switch (app->subState) {
    case 0:
        if (ov80_02237D8C(app->challengeType) == 1) {
            sub_02037BEC();
            sub_02037AC0(215);
        }

        app->subState++;
        break;
    case 1:
        if (ov80_02237D8C(app->challengeType) == 1) {
            if (sub_02037B38(215) == 1) {
                sub_02037BEC();
                app->subState++;
            }
        } else {
            app->subState++;
        }
        break;
    case 2:
        if (ov80_02237D8C(app->challengeType) == 1) {
            if (ov83_02241368(app, 10, 0) == 1) {
                app->subState++;
            }
        } else {
            ov83_0223E10C(app);
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, (((0) << 10) | ((0) << 5) | (0)), 6, 3, HEAP_ID_107);
            app->subState++;
        }
        break;
    case 3:
        if (ov80_02237D8C(app->challengeType) == 1) {
            if (app->msgsReceived >= 2) {
                app->msgsReceived = 0;
                ov83_0223E10C(app);
                BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, (((0) << 10) | ((0) << 5) | (0)), 6, 3, HEAP_ID_107);
                app->subState++;
            }
        } else {
            app->subState++;
        }
        break;
    case 4:
        if (IsPaletteFadeFinished() == 1) {
            return 1;
        }
        break;
    }

    return 0;
}
