#include "global.h"
#include "sprite_resources_header.h"

#include "overlay_70_gtsapplication_state.h"

#include "unk_0203A3B0.h"
#include "unk_0202C034.h"
#include "options.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "pokemon_summary_app.h"
#include "evolution_data.h"
#include "list_menu.h"
#include "sav_chatot.h"
#include "overlay_manager.h"

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


typedef struct GTSPlayerData GTSPlayerData;





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



enum GTSWFCInitScreenStates {
    GTSWFCINIT_ASK_TO_SETUP_CONNECTION,
    GTSWFCINIT_PROCESS_SETUP_CONFIRMATION,
    GTSWFCINIT_CONNECT,
    GTSWFCINIT_CHECK_CONNECTION,
    GTSWFCINIT_AUTHENTICATE,
    GTSWFCINIT_CHECK_AUTHENTICATION,
    GTSWFCINIT_GET_DWC_KEY,
    GTSWFCINIT_CONNECT_TO_REMOTE_SERVER,
    GTSWFCINIT_WAIT_FOR_SERVER_RESPONSE,
    GTSWFCINIT_SET_PROFILE_REQUEST,
    GTSWFCINIT_SET_PROFILE_RESPONSE,
    GTSWFCINIT_EXIT_SCREEN,
    GTSWFCINIT_WAIT_FOR_TEXT,
    GTSWFCINIT_WAIT_FOR_TEXT_THEN_YES_NO_MENU,
    GTSWFCINIT_HANDLE_RETRY_MENU_INPUT,
    GTSWFCINIT_ASK_TO_DISCONNECT,
    GTSWFCINIT_RESTART_OR_EXIT,
    GTSWFCINIT_SHOW_DISCONNECTING_MESSAGE,
    GTSWFCINIT_CLEANUP_NETWORKING,
    GTSWFCINIT_SHOW_DISCONNECTED_MESSAGE,
    GTSWFCINIT_WAIT_FOR_TEXT_DELAYED,
    GTSWFCINIT_FATAL_ERROR,
    GTSWFCINIT_FATAL_ERROR_DISCONNECT_MESSAGE,
    GTSWFCINIT_SHOW_NETWORK_ERROR,
    GTSWFCINIT_RESTART_CONNECTION
};

int ov73_021E8440(GTSApplicationState *);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(GTSApplicationState_Unk12D0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

static inline void inline_ov61_0222C0F8(GTSApplicationState_Unk12D0 *param0) {
    return;
}

static inline void inline_ov61_0222C3B0_sub_1(GTSApplicationState_Unk12D0 *param0) {
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
    GTSApplicationState_Unk12D0 *v0 = param1;

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

int GTSWFCInit_AskToSetupConnection(GTSApplicationState *appState);

int GTSWFCInit_ProcessSetupConfirmation(GTSApplicationState *appState);

int ov70_022447A0(GTSApplicationState *appState);

int ov70_02244888(GTSApplicationState *appState);

int ov70_022448C0(GTSApplicationState *appState);

int ov70_02244A04(GTSApplicationState *appState);

int ov70_02244A4C(GTSApplicationState *appState);

int ov70_02244B20(GTSApplicationState *appState);

int ov70_02244B54(GTSApplicationState *appState, s32 arg1);

int ov70_02244D08(GTSApplicationState *appState);

int GTSWFCInit_HandleRetryMenuInput(GTSApplicationState *appState);

int GTSWFCInit_WaitForText(GTSApplicationState *appState);

int GTSWFCInit_WaitForTextThenYesNoMenu(GTSApplicationState *appState);

int ov70_02244D80(GTSApplicationState *appState);

int GTSWFCInit_FatalErrorDisconnectMessage(GTSApplicationState *appState);

int ov70_02244A68(GTSApplicationState *appState);

int GTSWFCInit_AskToDisconnect(GTSApplicationState *appState);

int GTSWFCInit_ShowDisconnectingMessage(GTSApplicationState *appState);

int ov70_02244834(GTSApplicationState *appState);

int GTSWFCInit_ShowDisconnectedMessage(GTSApplicationState *appState);

int GTSWFCInit_WaitForTextDelayed(GTSApplicationState *appState);

int ov70_02244944(GTSApplicationState *appState);

int ov70_02244954(GTSApplicationState *appState);

int ov70_02244CA8(GTSApplicationState *appState);

int ov70_02244CCC(GTSApplicationState *appState);

extern int (*ov70_02246780[25])(GTSApplicationState *);

int ov70_02244124(GTSApplicationState *appState, int unused, s32 arg2);

int ov70_02238E44(GTSApplicationState *, int, s32);

int ov70_02244124(GTSApplicationState *appState, int unused, s32 arg2) {
    sub_0203A930(ov70_02238E44(appState, unused, arg2));

    int previousInstruction = appState->currentScreenInstruction;
    int parentStateCode = (*ov70_02246780[appState->currentScreenInstruction])(appState);

    if (previousInstruction != appState->currentScreenInstruction) {
        appState->wfcDisconnectMessageIndex = 0;
        appState->wfcDisconnectMessageFrameDelay = 0;
    }

    inline_ov61_0222C0F8(&appState->unk_12D0);

    return parentStateCode;
}
