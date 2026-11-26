#include "global.h"
#include "sprite_resources_header.h"

#include "overlay_70_gtsapplication_state.h"

#include "bg_window.h"
#include "text.h"
#include "unk_0202C034.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "evolution_data.h"
#include "list_menu.h"
#include "sav_chatot.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "gts_player_data.h"

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

struct TextPrinterTemplate;

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

typedef BOOL (*TextPrinterCallback)(TextPrinterTemplate *, u16);

typedef u32 TextColor;

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



void ov70_02239D44(void *arg0, s32 arg1);

void ov70_022394B8(GTSApplicationState *appState);

void ov70_022394B8(GTSApplicationState *appState) {
    AddWindowParameterized(appState->bgConfig, &appState->titleWindow, 1, 2, 1, 28, 2, 13, (1 + ((6 * 3) + ((2 * 2) * 3)) + (3 * 3)));
    FillWindowPixelBuffer(&appState->titleWindow, 0x0);
    AddTextPrinterParameterizedWithColor(&appState->titleWindow, FONT_MESSAGE, appState->title, 0, 2, 0, ((TextColor)(((15 & 0xFF) << 16) | ((6 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);

    for (int i = 0; i < 3; i++) {
        AddWindowParameterized(appState->bgConfig, &appState->menuButtonWindows[i], 1, 9, 6 + i * 5, 15, 2, 1, (16 * 6 + 1) + (15 * 2) * i);
        FillWindowPixelBuffer(&appState->menuButtonWindows[i], 0x0);
    }

    AddWindowParameterized(appState->bgConfig, &appState->bottomInstructionWindow, 0, 2, 21, 27, 2, 13, ((1 + ((6 * 3) + ((2 * 2) * 3)) + (3 * 3)) + (28 * 2)));
    FillWindowPixelBuffer(&appState->bottomInstructionWindow, 0xf0f);
    AddWindowParameterized(appState->bgConfig, &appState->confirmationWindow, 0, 2, 19, 27, 4, 13, ((1 + ((6 * 3) + ((2 * 2) * 3)) + (3 * 3)) + (28 * 2)) + (27 * 2));
    FillWindowPixelBuffer(&appState->confirmationWindow, 0xf0f);
    ov70_02239D44(appState, 0);
}
