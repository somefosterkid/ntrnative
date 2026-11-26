#include "global.h"

#include "overlay_70_gtsapplication_state.h"

#include "screen_fade.h"
#include "sprite.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_0202C034.h"
#include "options.h"
#include "pokemon_types_def.h"
#include "overlay_41_sprite_resource_collection.h"
#include "evolution_data.h"
#include "list_menu.h"
#include "sav_chatot.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "gts_player_data.h"

void ov70_022396A8(GTSApplicationState *arg0);

extern TouchscreenHitbox ov70_0224524C;
void ov70_02239730(void);

s32 ov70_02238E50(void *, s32, s32);
int ov70_02238E64(GTSApplicationState *appState);
s32 ov70_02239BDC(void *, s32, s32, s32, s32);
void ov70_02239740(GTSApplicationState *arg0, s32 arg1);

void ov70_022396A8(GTSApplicationState *arg0) {
    BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_61);
    arg0->currentScreenInstruction = 3;
}

struct OverlayManager;

struct OverlayManagerTemplate;

struct BgConfig;

struct BoxPokemon;

struct SOUND_CHATOT;

struct ColoredArrow;

struct DWCstInetControl;

struct EvolutionData;

struct G2dRenderer;

struct GTSApplicationState;

struct GTSBoxPokemonCriteria;

struct GTSCharpadScrollState;

struct GTSPlayerData;

struct GTSPokemonCriteria;

struct GTSPokemonListing;

struct GTSPokemonListingMon;

struct GTSPokemonRequirements;

struct GTSTradedPokemonLocation;

struct ListMenu;

struct ListMenuTemplate;

struct Menu;

struct MsgData;

struct Options;

struct Pokemon;


struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct ListMenuItem;

struct MessageFormat;

struct SysTask;

struct SysTaskQueue;

struct TradeAnimationTemplate;

struct PlayerProfile;

struct WiFiList;

struct Window;

struct WorldExchangeTrainer;

struct WorldExchangeTrainerError;

struct tagDWCWDSData;




typedef u16 charcode_t;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};


enum TradeType {
    TRADE_TYPE_NORMAL = 1,
    TRADE_TYPE_SEND_ONLY,
    TRADE_TYPE_RECEIVE_ONLY = 4,
};

enum TradeBackground {
    TRADE_BACKGROUND_DAY = 0,
    TRADE_BACKGROUND_EVENING,
    TRADE_BACKGROUND_NIGHT,
    TRADE_BACKGROUND_WIFI,
};


typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

typedef struct ColoredArrow ColoredArrow;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

typedef struct Menu Menu;

enum GTSApplicationLoopState {
    GTS_LOOP_STATE_WAIT_FOR_WIRELESS_DRIVER = 0,
    GTS_LOOP_STATE_INIT,
    GTS_LOOP_STATE_WAIT_FADE,
    GTS_LOOP_STATE_MAIN,
    GTS_LOOP_STATE_FINISH,
    GTS_LOOP_STATE_EXIT
};







typedef struct GTSBoxPokemonCriteria GTSBoxPokemonCriteria;



enum GTSMainMenuScreenStates {
    GTS_MAINMENU_WAIT_UNTIL_FINISHED_MOVING,
    GTS_MAINMENU_QUEUE_FADE_IN,
    GTS_MAINMENU_BEGIN_FADE_IN,
    GTS_MAINMENU_WAIT_FADE_IN_1,
    GTS_MAINMENU_WAIT_FADE_IN_2,
    GTS_MAINMENU_SETUP_BOTTOM_WINDOW_QUESTION,
    GTS_MAINMENU_HANDLE_INPUT,
    GTS_MAINMENU_BEGIN_LOGOUT_ANIM,
    GTS_MAINMENU_WAIT_LOGOUT_ANIM,
    GTS_MAINMENU_FADE_AND_EXIT,
    GTS_MAINMENU_WAIT_FOR_TEXT_PRINTER,
    GTS_MAINMENU_WAIT_FOR_MESSAGE_WITH_DELAY,
    GTS_MAINMENU_SHOW_CONFIRMATION_MENU,
    GTS_MAINMENU_HANDLE_CONFIRMATION_MENU,
};

int ov70_022396CC(GTSApplicationState *appState);

int ov70_022396CC(GTSApplicationState *appState) {
    if (IsPaletteFadeFinished()) {
        appState->currentScreenInstruction = GTS_MAINMENU_SETUP_BOTTOM_WINDOW_QUESTION;
    }

    return GTS_LOOP_STATE_MAIN;
}

int ov70_022396E0(GTSApplicationState *appState);

int ov70_022396E0(GTSApplicationState *appState) {
    if (IsPaletteFadeFinished()) {
        appState->currentScreenInstruction = GTS_MAINMENU_SETUP_BOTTOM_WINDOW_QUESTION;
    }

    return GTS_LOOP_STATE_MAIN;
}

s32 ov70_02238D84(void *, s32, s32);

s32 ov70_02238F10(void *);

s32 ov70_02239B84(void *, s32, s32, s32, s32);

s32 ov70_022396F4(GTSApplicationState *arg0);

s32 ov70_022396F4(GTSApplicationState *arg0) {
    ov70_02239B84(arg0, 4, 1, 0, 0xF0F);
    ov70_02238D84(arg0, 0xA, 6);
    Sprite_SetAnimActiveFlag(arg0->cursorSprite, 1);
    ov70_02238F10(arg0);
    return 3;
}

void ov70_02239730(void) {
    TouchscreenHitbox_FindRectAtTouchNew(&ov70_0224524C);
}

void ov70_02239740(GTSApplicationState *arg0, s32 arg1) {
    switch (arg1) {
    case 0:
        if (arg0->isPokemonListed == 0) {
            ov70_02238E50(arg0, 5, 5);
            arg0->currentScreenInstruction = 9;
            PlaySE(0x5DCU);
            return;
        }
        if (arg0->networkTimer == 0) {
            ov70_02238E50(arg0, 7, 0xB);
            arg0->returnAfterNetworkScreen = 2;
            arg0->currentScreenInstruction = 9;
            arg0->networkTimer = 0x708U;
            PlaySE(0x5DCU);
            return;
        }
        Sprite_SetAnimActiveFlag(arg0->cursorSprite, 0);
        ov70_02239B84(arg0, 0x22, 1, 0, 0xF0F);
        ov70_02238D84(arg0, 0xB, 5);
        PlaySE(0x5F3U);
        arg0->unk_11C0 = 0;
        return;
    case 1:
        arg0->searchResultsVisible = 0;
        ov70_02238E50(arg0, 4, 0xD);
        arg0->currentScreenInstruction = 9;
        PlaySE(0x5DCU);
        return;
    case 2:
        ov70_02239BDC(arg0, 7, ov70_02238E64(arg0), 0, 0xF0F);
        ov70_02238D84(arg0, 0xA, 0xC);
        Sprite_SetAnimActiveFlag(arg0->cursorSprite, 0);
        PlaySE(0x5DCU);
        return;
    }
}
