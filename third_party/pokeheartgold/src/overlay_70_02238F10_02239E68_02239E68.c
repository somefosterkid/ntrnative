#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_70_gtsapplication_state.h"
#include "unk_0202C034.h"
#include "options.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "pokemon_summary_app.h"
#include "evolution_data.h"
#include "list_menu.h"
#include "sav_chatot.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

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

struct PokemonSummary;

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


typedef struct GTSPlayerData GTSPlayerData;





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







typedef struct GTSBoxPokemonCriteria GTSBoxPokemonCriteria;


enum GTSListingStateHandlers {
    GTSLISTING_WAIT_FADE_IN,
    GTSLISTING_HANDLE_INPUT,
    GTSLISTING_BEGIN_EXIT,
    GTSLISTING_WAIT_FOR_MESSAGE,
    GTSLISTING_WAIT_FOR_MESSAGE_WITH_DELAY,
    GTSLISTING_SHOW_CONFIRMATION_MENU,
    GTSLISTING_HANDLE_CONFIRMATION_MENU,
    GTSLISTING_SHOW_ACTION_MENU,
    GTSLISTING_HANDLE_ACTION_MENU
};

int ov70_0223A258(GTSApplicationState *appState);

int GTSListing_HandleInput(GTSApplicationState *appState);

int GTSListing_BeginExit(GTSApplicationState *appState);

int GTSListing_WaitForMessage(GTSApplicationState *appState);

int GTSListing_WaitForMessageWithDelay(GTSApplicationState *appState);

int GTSListing_ShowConfirmationMenu(GTSApplicationState *appState);

int GTSListing_HandleConfirmationMenu(GTSApplicationState *appState);

int GTSListing_ShowActionMenu(GTSApplicationState *appState);

int GTSListing_HandleActionMenu(GTSApplicationState *appState);

extern int (*ov70_022464A8[9])(GTSApplicationState *);

int ov70_02239E68(GTSApplicationState *appState, int unused);

int ov70_02239E68(GTSApplicationState *appState, int unused) {
    return (*ov70_022464A8[appState->currentScreenInstruction])(appState);
}
