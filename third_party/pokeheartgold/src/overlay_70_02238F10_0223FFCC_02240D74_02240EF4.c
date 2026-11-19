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

struct AvatarAnimationData;

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


void *SysTask_GetData(SysTask *task);


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


void ov70_02240D74(GTSApplicationState *appState, int gender, s32, s32);

BOOL PlaySE(u16 seqID);

SysTask *CreateSysTaskAndEnvironment(SysTaskFunc callback, int paramSize, u32 priority, enum HeapID heapID);

typedef struct AvatarAnimationData {
    int state;
    int y;
    int gender;
    GTSApplicationState *appState;
} AvatarAnimationData;

enum AvatarAnimState {
    AVATARANIM_INITIAL_SLIDE,
    AVATARANIM_WAIT_FOR_ANIM,
    AVATARANIM_SECONDARY_SLIDE,
    AVATARANIM_FINISH
};

void ov70_02240F58(SysTask *task, void *param);

void ov70_02240F3C(AvatarAnimationData *animData, int animIndex);

void ov70_02240EF4(GTSApplicationState *appState, int gender, s32 arg2, s32 arg3);

void ov70_02240EF4(GTSApplicationState *appState, int gender, s32 arg2, s32 arg3) {
    ov70_02240D74(appState, gender, arg2, arg3);
    AvatarAnimationData *animData;
    appState->playerAvatarAnimationTask = CreateSysTaskAndEnvironment(ov70_02240F58, sizeof(AvatarAnimationData), 5, (enum HeapID)61);

    animData = SysTask_GetData(appState->playerAvatarAnimationTask);
    animData->state = AVATARANIM_INITIAL_SLIDE;
    animData->y = -40;
    animData->gender = gender;
    animData->appState = appState;

    ov70_02240F3C(animData, 0);
    PlaySE(1582);
}
