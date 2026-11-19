#include "global.h"
#include "sprite_resources_header.h"

#include "overlay_70_gtsapplication_state.h"
#include "unk_0202C034.h"
#include "options.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "evolution_data.h"
#include "list_menu.h"

struct AffineSpriteListTemplate;
struct ApplicationManager;
struct ApplicationManagerTemplate;
struct BgConfig;
struct BoxPokemon;
struct ChatotCry;
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
struct MessageLoader;
struct NARC;
struct Options;
struct Pokemon;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct SpriteResourceCollection;
struct SpriteResourcesHeader;
struct String;
struct StringList;
struct StringTemplate;
struct SysTask;
struct SysTaskManager;
struct TradeAnimationTemplate;
struct TrainerInfo;
struct WiFiList;
struct Window;
struct WorldExchangeTrainer;
struct WorldExchangeTrainerError;
struct tagDWCWDSData;
void Heap_Free(void *ptr);
typedef struct ChatotCry ChatotCry;
typedef u16 charcode_t;
void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);
typedef struct SysTaskManager SysTaskManager;
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};
typedef struct AffineSpriteListTemplate AffineSpriteListTemplate;
typedef struct TrainerInfo TrainerInfo;
typedef struct GTSPlayerData GTSPlayerData;
typedef struct MessageLoader MessageLoader;
typedef struct StringTemplate StringTemplate;
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
typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;
typedef struct ApplicationManager ApplicationManager;
typedef BOOL (*OverlayFunc)(ApplicationManager *appMan, int *state);
typedef struct ApplicationManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} ApplicationManagerTemplate;
struct ApplicationManager {
    ApplicationManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    ApplicationManager *parent;
    ApplicationManager *child;
};
typedef struct ColoredArrow ColoredArrow;
typedef struct StringList StringList;
typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);
typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);
typedef struct Menu Menu;
typedef struct GTSBoxPokemonCriteria GTSBoxPokemonCriteria;
enum ScreenArgument {
    SCREEN_ARGUMENT_NONE = 0,
    SCREEN_ARGUMENT_RESUME_LISTING = 3,
    SCREEN_ARGUMENT_CHOOSE_OFFER_MON = 5,
    SCREEN_ARGUMENT_CHOOSE_LISTED_MON,
    SCREEN_ARGUMENT_DEPOSIT_POKEMON,
    SCREEN_ARGUMENT_TAKE_BACK_POKEMON,
    SCREEN_ARGUMENT_EXCHANGE_POKEMON,
    SCREEN_ARGUMENT_EXCHANGE_POKEMON_ALT,
    SCREEN_ARGUMENT_CHECK_SERVER,
    SCREEN_ARGUMENT_SAVE_AFTER_EVOLVE,
};
int GTSApplication_Deposit_Init(GTSApplicationState *appState, int unused);
int GTSApplication_Deposit_Main(GTSApplicationState *appState, int unused);
int GTSApplication_Deposit_Exit(GTSApplicationState *appState, int unused);
int GTSApplication_Listing_Init(GTSApplicationState *appState, int unused);
int ov70_02239E68(GTSApplicationState *appState, int unused);
int GTSApplication_Listing_Exit(GTSApplicationState *appState, int unused);
int GTSApplication_MainMenu_Init(GTSApplicationState *appState, int sLotoId);
int GTSApplication_MainMenu_Main(GTSApplicationState *appState, int sLotoId);
int GTSApplication_MainMenu_Exit(GTSApplicationState *appState, int sLotoId);
int GTSApplication_NetworkHandler_Init(GTSApplicationState *appState, int unused);
int GTSApplication_NetworkHandler_Main(GTSApplicationState *appState, int unused);
int GTSApplication_NetworkHandler_Exit(GTSApplicationState *appState, int unused);
int GTSApplication_PokemonSummary_Init(GTSApplicationState *appState, int unused);
int GTSApplication_PokemonSummary_Main(GTSApplicationState *appState, int unused);
int GTSApplication_PokemonSummary_Exit(GTSApplicationState *appState, int unused);
int GTSApplication_Search_Init(GTSApplicationState *appState, int unused);
int GTSApplication_Search_Main(GTSApplicationState *appState, int unused);
int GTSApplication_Search_Exit(GTSApplicationState *appState, int unused);
int GTSApplication_SearchListing_Init(GTSApplicationState *appState, int unused);
int GTSApplication_SearchListing_Main(GTSApplicationState *appState, int unused);
int GTSApplication_SearchListing_Exit(GTSApplicationState *appState, int unused);
int GTSApplication_SelectMon_Init(GTSApplicationState *param0, int param1);
int GTSApplication_SelectMon_Main(GTSApplicationState *param0, int param1);
int GTSApplication_SelectMon_Exit(GTSApplicationState *param0, int param1);
int GTSApplication_Trade_Init(GTSApplicationState *appState, int unused);
int GTSApplication_Trade_Main(GTSApplicationState *appState, int unused);
int GTSApplication_Trade_Exit(GTSApplicationState *appState, int unused);
int GTSApplication_WFC_Init(GTSApplicationState *appState, int unused);
int GTSApplication_WFCInit_Main(GTSApplicationState *appState, int unused);
int GTSApplication_WFCInit_Exit(GTSApplicationState *appState, int unused);
SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);
static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);

void ov70_02238F10(GTSApplicationState *arg0);

static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID) {
    NNSG2dPaletteData *v0;
    void *v1;

    MI_CpuClear8(param0, sizeof(UnkStruct_ov61_0222C3B0));

    v1 = GfGfxLoader_GetPlttDataFromOpenNarc(param1, param2, &v0, heapID);

    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_08, 4 * 0x20);
    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_88, 4 * 0x20);
    Heap_Free(v1);
    inline_ov61_0222C3B0_sub_1(param0);

    param0->unk_04 = 1;
    param0->unk_00 = SysTask_ExecuteOnVBlank(inline_ov61_0222C3B0_sub, param0, 20);
}
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
void GTSApplication_InitAffineTemplate(AffineSpriteListTemplate *template, GTSApplicationState *appState, SpriteResourcesHeader *spriteResourceHeader, enum NNS_G2D_VRAM_TYPE vramType);
void GTSApplication_SetNextScreenWithArgument(GTSApplicationState *appState, int nextScreen, enum ScreenArgument screenArgument);

void ov70_02238F04(GTSApplicationState *appState);

void ov70_02238F04(GTSApplicationState *appState) {
    if (appState->networkTimer) {
        appState->networkTimer--;
    }
}

void ov70_02238F10(GTSApplicationState *arg0) {
    *((s16 *)&arg0->deferredBoxId) = 1;
    *((s16 *)&arg0->deferredBoxPokemonCount) = 0;
}
