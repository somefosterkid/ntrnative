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

enum GTSApplicationLoopState {
    GTS_LOOP_STATE_WAIT_FOR_WIRELESS_DRIVER = 0,
    GTS_LOOP_STATE_INIT,
    GTS_LOOP_STATE_WAIT_FADE,
    GTS_LOOP_STATE_MAIN,
    GTS_LOOP_STATE_FINISH,
    GTS_LOOP_STATE_EXIT
};







typedef struct GTSBoxPokemonCriteria GTSBoxPokemonCriteria;


enum GTSNetworkHandlerScreenStates {
    GTS_NETHANDLER_PARSE_SCREEN_ARGUMENT,
    GTS_NETHANDLER_IDLE,
    GTS_NETHANDLER_BEGIN_POST_LISTING,
    GTS_NETHANDLER_WAIT_FOR_POST_RESPONSE,
    GTS_NETHANDLER_FINISH_POST,
    GTS_NETHANDLER_POST_FINISH_RESPONSE,
    GTS_NETHANDLER_TRADE_DEPOSIT,
    GTS_NETHANDLER_GET_LISTED_POKEMON_REQUEST,
    GTS_NETHANDLER_GET_LISTED_POKEMON_RESPONSE,
    GTS_NETHANDLER_RETURN_REQUEST,
    GTS_NETHANDLER_RETURN_RESPONSE,
    GTS_NETHANDLER_TRADE_TAKE_BACK,
    GTS_NETHANDLER_EXCHANGE_REQUEST,
    GTS_NETHANDLER_EXCHANGE_RESPONSE,
    GTS_NETHANDLER_EXCHANGE_FINISH_REQUEST,
    GTS_NETHANDLER_EXCHANGE_FINISH_RESPONSE,
    GTS_NETHANDLER_GO_TO_TRADE_DEPOSIT,
    GTS_NETHANDLER_SHOW_ALREADY_TRADED_ERROR,
    GTS_NETHANDLER_PERFORM_DEPOSIT_TRADE,
    GTS_NETHANDLER_DELETE_RECEIVED_POKEMON_REQUEST,
    GTS_NETHANDLER_DELETE_RECEIVED_POKEMON_RESPONSE,
    GTS_NETHANDLER_GO_TO_TRADE_TAKE_BACK,
    GTS_NETHANDLER_DELETE_DESYNCED_POKEMON_REQUEST,
    GTS_NETHANDLER_DELETE_DESYNCED_POKEMON_RESPONSE,
    GTS_NETHANDLER_GET_LISTING_STATUS_REQUEST,
    GTS_NETHANDLER_GET_LISTING_STATUS_RESPONSE,
    GTS_NETHANDLER_GET_LISTED_POKEMON_REQUEST_2,
    GTS_NETHANDLER_GET_LISTED_POKEMON_RESPONSE_2,
    GTS_NETHANDLER_RETURN_TO_MAIN_MENU,
    GTS_NETHANDLER_BEGIN_SAVE_SEQUENCE,
    GTS_NETHANDLER_PREPARE_FULL_SAVE,
    GTS_NETHANDLER_WAIT_FOR_FRAME_DELAY,
    GTS_NETHANDLER_WAIT_FOR_SAVE_PROCEED,
    GTS_NETHANDLER_WAIT_FOR_SAVE_COMPLETE,
    GTS_NETHANDLER_FULL_SAVE,
    GTS_NETHANDLER_WAIT_FOR_SUCCESSFUL_SAVE,
    GTS_NETHANDLER_FADE_AND_EXIT,
    GTS_NETHANDLER_WAIT_FOR_TEXT,
    GTS_NETHANDLER_SHOW_COMMS_ERROR_AND_DISCONNECT,
    GTS_NETHANDLER_SHOW_COMMS_ERROR_AND_RETURN_TO_MENU,
};

BOOL ov70_02237F38(void);

s32 ov70_02237F58(void);

enum GTSNetworkResult {
    GTS_RESULT_SUCCESS = 0,
    GTS_RESULT_PROFILE_EXISTS = 1,
    GTS_RESULT_BANNED = 2,
    GTS_RESULT_1_FOUND = 1,
    GTS_RESULT_2_FOUND = 2,
    GTS_RESULT_3_FOUND = 3,
    GTS_RESULT_4_FOUND = 4,
    GTS_RESULT_5_FOUND = 5,
    GTS_RESULT_6_FOUND = 6,
    GTS_RESULT_7_FOUND = 7,

    GTS_RESULT_GTS_CROWDED = -1,
    GTS_RESULT_SERVER_MAINTENANCE = -2,
    GTS_RESULT_ALREADY_TRADED = -3,
    GTS_RESULT_NOT_FOUND = -4,
    GTS_RESULT_ALREADY_DEPOSITED = -5,
    GTS_RESULT_CANNOT_BE_OFFERED_1 = -6,
    GTS_RESULT_CANNOT_BE_OFFERED_2 = -7,
    GTS_RESULT_CANNOT_BE_OFFERED_3 = -8,
    GTS_RESULT_CANNOT_BE_OFFERED_4 = -9,
    GTS_RESULT_CANNOT_BE_OFFERED_5 = -10,
    GTS_RESULT_CANNOT_BE_OFFERED_6 = -11,
    GTS_RESULT_ABORTED = -12,
    GTS_RESULT_ERROR = -13,
    GTS_RESULT_NOT_IN_DCF_PHASE = -14,
    GTS_RESULT_CONNECTION_FAILED = -15,
};

void sub_020399EC(void);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

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

void ov70_022404D4(GTSApplicationState *appState);

BOOL ov70_02240D54(GTSApplicationState *appState);

int ov70_02240430(GTSApplicationState *appState);

int ov70_02240430(GTSApplicationState *appState) {
    if (ov70_02237F38()) {
        s32 errCode = ov70_02237F58();

        appState->networkTimeoutCounter = 0;

        switch (errCode) {
        case GTS_RESULT_SUCCESS:
            if (ov70_02240D54(appState)) {
                appState->currentScreenInstruction = GTS_NETHANDLER_DELETE_DESYNCED_POKEMON_REQUEST;
                appState->isPokemonListed = 0;
                return GTS_LOOP_STATE_MAIN;
            } else {
                appState->isPokemonListed = 1;
            }
            break;
        case GTS_RESULT_ALREADY_TRADED:
            appState->isPokemonListed = 0;
            break;
        case GTS_RESULT_NOT_FOUND:
            break;
        case GTS_RESULT_ABORTED:
        case GTS_RESULT_CONNECTION_FAILED:
        case GTS_RESULT_SERVER_MAINTENANCE:
        case GTS_RESULT_NOT_IN_DCF_PHASE:
            appState->commsErrorMessage = errCode;
            appState->currentScreenInstruction = GTS_NETHANDLER_SHOW_COMMS_ERROR_AND_DISCONNECT;
            return GTS_LOOP_STATE_MAIN;
        case GTS_RESULT_ERROR:
            sub_020399EC();
            return GTS_LOOP_STATE_MAIN;
        }

        ov70_022404D4(appState);
    } else {
        appState->networkTimeoutCounter++;

        if (appState->networkTimeoutCounter == (30 * 60 * 2)) {
            sub_020399EC();
        }
    }

    return GTS_LOOP_STATE_MAIN;
}
