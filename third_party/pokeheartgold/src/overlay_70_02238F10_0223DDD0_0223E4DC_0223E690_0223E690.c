#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_70_gtsapplication_state.h"
#include "unk_0202C034.h"
#include "options.h"
#include "seal_case.h"
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

struct BallCapsule;

struct BallSeal;

struct BgConfig;

struct BoxPokemon;

struct SOUND_CHATOT;

struct ColoredArrow;

struct DWCstInetControl;

struct MailMessage;

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

struct Mail;

struct Menu;

struct MsgData;

struct Options;

struct PartyPokemon;

struct Pokemon;

struct PokemonDataBlockA;

struct PokemonDataBlockB;

struct PokemonDataBlockC;

struct PokemonDataBlockD;

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





typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

typedef u16 charcode_t;

typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};


u32 GetBoxMonData(BoxPokemon *boxMon, int param, void *dest);

u32 CalcBoxMonLevel(BoxPokemon *boxMon);

BoxPokemon *Mon_GetBoxMon(Pokemon *mon);

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


void ov70_0223F6E4(GTSPokemonListing *listing, GTSApplicationState *appState);

void ov70_0223E690(GTSPokemonListing *listing, GTSApplicationState *appState);

void ov70_0223E690(GTSPokemonListing *listing, GTSApplicationState *appState) {
    GTSPokemonCriteria criteria;
    GTSPokemonRequirements requirements;
    BoxPokemon *boxMon;

    criteria.species = GetBoxMonData(appState->selectedBoxMon, MON_DATA_SPECIES, 0L);
    criteria.gender = GetBoxMonData(appState->selectedBoxMon, MON_DATA_GENDER, 0L) + 1;
    criteria.level = CalcBoxMonLevel(appState->selectedBoxMon);

    listing->criteria = criteria;

    ov70_0223F6E4(listing, appState);

    boxMon = Mon_GetBoxMon((Pokemon *)appState->searchResults[appState->selectedSearchResult].pokemon.bytes);

    requirements.species = GetBoxMonData(boxMon, MON_DATA_SPECIES, 0L);
    requirements.gender = GetBoxMonData(boxMon, MON_DATA_GENDER, 0L) + 1;
    requirements.level = 0;
    requirements.level2 = 0;

    listing->requirements = requirements;
}
