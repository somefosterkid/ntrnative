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

struct GTSCountrySearchRequest;

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

struct NARC;

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




void Heap_Free(void *ptr);

typedef u16 charcode_t;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);


enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};


void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

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



enum GTSSearchStateHandlers {
    GTS_SEARCH_SHOW_PROMPT,
    GTS_SEARCH_HANDLE_INPUT,
    GTS_SEARCH_FADE_AND_EXIT,
    GTS_SEARCH_SHOW_SPECIES_SEARCH_PROMPT,
    GTS_SEARCH_CREATE_CHARPAD,
    GTS_SEARCH_HANDLE_CHARPAD_INPUT,
    GTS_SEARCH_CREATE_SPECIES_MENU,
    GTS_SEARCH_HANDLE_SPECIES_MENU_INPUT,
    GTS_SEARCH_SHOW_GENDER_SEARCH_PROMPT,
    GTS_SEARCH_CREATE_GENDER_MENU,
    GTS_SEARCH_HANDLE_GENDER_MENU_INPUT,
    GTS_SEARCH_SHOW_LEVEL_SEARCH_PROMPT,
    GTS_SEARCH_CREATE_LEVEL_MENU,
    GTS_SEARCH_HANDLE_LEVEL_MENU_INPUT,
    GTS_SEARCH_SHOW_COUNTRY_SEARCH_PROMPT,
    GTS_SEARCH_CREATE_COUNTRY_MENU,
    GTS_SEARCH_HANDLE_COUNTRY_MENU_INPUT,
    GTS_SEARCH_SUBMIT_SEARCH,
    GTS_SEARCH_SEND_SEARCH_REQUEST,
    GTS_SEARCH_WAIT_FOR_SEARCH_RESULTS,
    GTS_SEARCH_SHOW_RESULTS_MESSAGE,
    GTS_SEARCH_WAIT_FOR_RESULTS_REVEAL,
    GTS_SEARCH_SHOW_INVALID_SEARCH_MESSAGE,
    GTS_SEARCH_WAIT_FOR_TEXT,
    GTS_SEARCH_WAIT_FOR_TEXT_WITH_DELAY,
    GTS_SEARCH_CREATE_EXIT_CONFIRM_MENU,
    GTS_SEARCH_HANDLE_EXIT_CONFIRM,
    GTS_SEARCH_IDLE,
    GTS_SEARCH_SHOW_SEARCH_PROMPT_AGAIN,
    GTS_SEARCH_SHOW_SERVER_MAINTENANCE_MESSAGE,
    GTS_SEARCH_DISCONNECT_AND_EXIT
};

typedef struct GTSCountrySearchRequest {
    s16 species;
    s8 gender;
    s8 level;
    s8 level2;
    s8 unk_05;
    s8 count;
    u8 countryIndex;
} GTSCountrySearchRequest;

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov70_02238130(const GTSPokemonRequirements *requirements, s8 maxResults, GTSPokemonListing *listing);
#else
void ov70_02238130(const GTSPokemonRequirements *requirements, s32 maxResults, GTSPokemonListing *listing);
#endif

void ov70_022381A4(const GTSCountrySearchRequest *req, GTSPokemonListing *listing);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

int ov70_0223BFA8(GTSApplicationState *appState, int bySearching);

int ov70_0223C008(GTSApplicationState *appState);

int ov70_0223C008(GTSApplicationState *appState) {
    int maxResults = 3;

    if (ov70_0223BFA8(appState, 1)) {
        maxResults += 2;
    }

    if (ov70_0223BFA8(appState, 0)) {
        maxResults += 2;
    }

    if (appState->selectedCountryIndex == 0) {
        ov70_02238130(&appState->searchRequirements, maxResults, appState->searchResults);
    } else {
        GTSCountrySearchRequest request;

        MI_CpuClear8(&request, sizeof(GTSCountrySearchRequest));

        request.species = appState->searchRequirements.species;
        request.gender = appState->searchRequirements.gender;
        request.level = appState->searchRequirements.level;
        request.level2 = appState->searchRequirements.level2;
        request.unk_05 = appState->searchRequirements.unset_05;
        request.count = maxResults;
        request.countryIndex = appState->selectedCountryIndex;

        ov70_022381A4(&request, appState->searchResults);
    }

    appState->submittedRequirements = appState->searchRequirements;
    appState->submittedCountryIndex = appState->selectedCountryIndex;
    appState->networkTimeoutCounter = 0;
    appState->currentScreenInstruction = (enum GTSSearchStateHandlers)17;
    appState->searchResultsVisible = 0;

    return 3;
}
