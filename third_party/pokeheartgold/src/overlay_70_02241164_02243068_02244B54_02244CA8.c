#include "global.h"
struct GTSApplicationState;

#include "gf_gfx_loader.h"
#include "overlay_00_thumb.h"
#include "overlay_manager.h"
#include "render_window.h"
#include "screen_fade.h"
#include "unk_0203A3B0.h"
#include "yes_no_prompt.h"
#include "overlay_70_gtsapplication_state.h"
#include "overlay_41_sprite_resource_collection.h"
#include "pokemon_summary_app.h"
#include "evolution_data.h"
#include "sav_chatot.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct ApplicationManager;

struct SOUND_CHATOT;

struct EvolutionData;

struct GTSBoxPokemonCriteria;

struct GTSPlayerData;

struct Menu;

struct MsgData;

struct SpriteResourceCollection;

struct ListMenuItem;

struct MessageFormat;

struct PlayerProfile;

typedef u16 charcode_t;

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


typedef struct ApplicationManager ApplicationManager;

struct ApplicationManager {
    OverlayManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    ApplicationManager *parent;
    ApplicationManager *child;
};

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

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);

static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);

s32 ov70_02244CCC(GTSApplicationState *arg0);
s32 ov70_02238F80(void *);
s32 ov70_02244D08(struct GTSApplicationState *arg0);
s32 ov70_02238C8C(GTSApplicationState *arg0);
s32 ov70_02238E50(void *, s32, s32);
s32 ov70_02244D44(GTSApplicationState *arg0);

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

void ov70_022451A8(GTSApplicationState *appState, int errorCode, int visibleErrorCode);

int ov70_02244CA8(GTSApplicationState *appState);

int ov70_02244CA8(GTSApplicationState *appState) {
    int errorMessage = ov00_021E6A70(-appState->dwcErrCode, appState->dwcErrType);
    ov70_022451A8(appState, errorMessage, -appState->dwcErrCode);
    appState->currentScreenInstruction = GTSWFCINIT_RESTART_CONNECTION;

    return GTS_LOOP_STATE_MAIN;
}

s32 ov70_02244CCC(GTSApplicationState *arg0) {
    if ((1 & gSystem.newKeys) || (2 & gSystem.newKeys) || (gSystem.touchNew != 0)) {
        ClearWindowPixelBuffer(&arg0->networkErrWindow, 0);
        arg0->currentScreenInstruction = 0;
    }
    return 3;
}

s32 ov70_02244D08(GTSApplicationState *arg0) {
    sub_0203A914();
    ov70_02238F80(arg0);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_61);
    *((s32 *)&arg0->currentScreenInstruction) = 0;
    *((s32 *)&arg0->fadeBothScreens) = 1;
    return 4;
}

s32 ov70_02244D44(GTSApplicationState *arg0) {
    s32 v0;

    v0 = ov70_02238C8C(arg0);
    if (v0 == 1) {
        YesNoPrompt_Destroy(arg0->unk_11C8);
        ov70_02238E50(arg0, 0, 0);
        arg0->currentScreenInstruction = 0xB;
    } else if (v0 == 2) {
        YesNoPrompt_Destroy(arg0->unk_11C8);
        arg0->currentScreenInstruction = 0;
    }
    return 3;
}
