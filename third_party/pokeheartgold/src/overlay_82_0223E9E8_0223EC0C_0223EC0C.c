#include "global.h"

#include "bg_window.h"
#include "palette.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"
#include "overlay_82_battle_hall_app.h"
#include "pm_string.h"
#include "save.h"
#include "options.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "pokemon_summary_app.h"
#include "message_format.h"
#include "overlay_manager.h"
#include "message_printer.h"
#include "msgdata.h"
#include "list_menu_items.h"
#include "gf_gfx_planes.h"

struct BattleHallApp;

struct BattleHallAppSprite;

struct BattleHallAppSpriteManager;

struct G2dRenderer;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct ListMenuItem;

struct Window;

typedef struct BattleHallAppSpriteManager {
    SpriteList *spriteList;
    G2dRenderer renderer;
    SpriteResourceCollection *resourceCollection[4];
    SpriteResource *resources[2][4];
} BattleHallAppSpriteManager;

typedef struct BattleHallAppSprite BattleHallAppSprite;

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

void ov82_0223EDF0(BattleHallApp *app, enum GFBgLayer bgLayer);

void LoadSubScreenBackground(BattleHallApp *app, enum GFBgLayer bgLayer);

void ChangeState(BattleHallApp *app, int *state, enum BattleHallAppState nextState);

void ov82_0223EC0C(void *data);

void ov82_0223EC0C(void *data) {
    BattleHallApp *app = data;

    if (app->monSummaryAppMan != NULL) {
        return;
    }

    if (app->plttData != NULL) {
        PaletteData_PushTransparentBuffers(app->plttData);
    }

    DoScheduledBgGpuUpdates(app->bgConfig);
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
