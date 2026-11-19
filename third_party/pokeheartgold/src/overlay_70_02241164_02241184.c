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


void Sprite_SetDrawFlag(Sprite *sprite, BOOL draw);



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



BOOL PlaySE(u16 seqID);

void ov70_022412C8(NNSG2dCharacterData *charData, NNSG2dPaletteData *plttData, int slotIndex, int appearance, int gender);

void ov70_02241184(GTSApplicationState *appState, int resultCount, int showIdle);

void ov70_02241184(GTSApplicationState *appState, int resultCount, int showIdle) {
    if ((resultCount != 0) && (showIdle == 1)) {
        PlaySE(1614);
    }

    for (int i = 0; i < 7; i++) {
        if (i < resultCount) {
            int appearance = appState->searchResults[i].trainerAppearance;
            int gender = appState->searchResults[i].trainerGender;

            ov70_022412C8(appState->avatarCharData, appState->avatarPaletteData, i, appearance, gender);

            if (showIdle) {
                Sprite_SetAnimCtrlSeq(appState->avatarSprites[i + 1], 14 + i * 4);
            } else {
                Sprite_SetAnimCtrlSeq(appState->avatarSprites[i + 1], 17 + i * 4);
            }

            Sprite_SetDrawFlag(appState->avatarSprites[i + 1], 1);
        } else {
            Sprite_SetDrawFlag(appState->avatarSprites[i + 1], 0);
        }
    }
}
