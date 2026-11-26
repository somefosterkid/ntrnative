#include "global.h"

#include "game_stats.h"
#include "gf_gfx_loader.h"
#include "overlay_manager.h"
#include "pokemon.h"
#include "pokewalker.h"
#include "unk_02037C94.h"
#include "overlay_70_gtsapplication_state.h"
#include "overlay_41_sprite_resource_collection.h"
#include "pokemon_summary_app.h"
#include "evolution_data.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "gts_player_data.h"

struct OverlayManager;

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


typedef struct Menu Menu;

enum GTSApplicationScreen {
    GTS_SCREEN_WFC_INIT = 0,
    GTS_SCREEN_MAIN_MENU,
    GTS_SCREEN_LISTING,
    GTS_SCREEN_SEARCH_LISTING,
    GTS_SCREEN_SEARCH,
    GTS_SCREEN_SELECT_POKEMON,
    GTS_SCREEN_DEPOSIT,
    GTS_SCREEN_NETWORK_HANDLER,
    GTS_SCREEN_POKEMON_SUMMARY,
    GTS_SCREEN_TRADE
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

void ov70_02238E50(GTSApplicationState *appState, int nextScreen, enum ScreenArgument screenArgument);

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

void ov70_022404D4(GTSApplicationState *appState) {
    switch (appState->returnAfterNetworkScreen) {
    case GTS_SCREEN_MAIN_MENU:
        ov70_02238E50(appState, GTS_SCREEN_MAIN_MENU, SCREEN_ARGUMENT_NONE);
        appState->currentScreenInstruction = GTS_NETHANDLER_FADE_AND_EXIT;
        break;
    case GTS_SCREEN_LISTING:
        ov70_02238E50(appState, GTS_SCREEN_LISTING, SCREEN_ARGUMENT_RESUME_LISTING);
        appState->currentScreenInstruction = GTS_NETHANDLER_FADE_AND_EXIT;
        break;
    }
}

void ov70_02240500(GTSApplicationState *arg0, Pokemon *arg1);

void ov70_02240500(GTSApplicationState *arg0, Pokemon *mon) {
    GameStats_AddScore(*(GameStats **)((u8 *)arg0->playerData + 0x28), 0x18);
    GameStats_Inc(*(GameStats **)((u8 *)arg0->playerData + 0x28), 0x19);
    if (gGameLanguage != GetMonData(mon, 0xC, NULL)) {
        Pokewalker_UnlockCourse(Save_Pokewalker_Get(*(SaveData **)((u8 *)arg0->playerData + 0x20)), 0x14);
    }
}

s32 ov70_02240A7C(void *, void *, s32, int);

void ov70_02240CE4(SaveWiFiHistory *, GTSPokemonListing *);

s32 ov70_02240D44(void *, s16, s16);

s32 sub_0202DB5C(void *, s16);

u16 sub_0202DBA0(void *);

s32 ov70_02240540(GTSApplicationState *arg0);

s32 ov70_02240540(GTSApplicationState *arg0) {
    ov70_02240A7C(arg0, &arg0->receivedListing, sub_0202DBA0(*(void **)arg0->playerData), arg0->receivedListing.exchangedFromRemote);
    ov70_02240CE4(*(void **)((u8 *)arg0->playerData + 0x18), &arg0->receivedListing);
    ov70_02240500(arg0, &arg0->receivedListing.pokemon);
    sub_0202DB5C(*(void **)(arg0->playerData), 0);
    arg0->currentScreenInstruction = 0x1E;
    ov70_02240D44(arg0, 0x13, 0xB);
    return 3;
}

s32 ov70_022380A8(void);

s32 ov70_02240598(GTSApplicationState *arg0);

s32 ov70_02240598(GTSApplicationState *arg0) {
    ov70_022380A8();
    arg0->currentScreenInstruction = 0x14;
    arg0->networkTimeoutCounter = 0;
    return 3;
}

s32 ov70_02237F38(void);

s32 ov70_02237F58(void);

s32 ov70_022405B4(GTSApplicationState *arg0);

s32 ov70_022405B4(GTSApplicationState *arg0) {
    s32 v6;
    u32 v7;

    if (ov70_02237F38() != 0) {
        v6 = ov70_02237F58();
        arg0->networkTimeoutCounter = 0;
        v7 = v6 + 0xF;
        switch (v7) {
        case 15:
            arg0->currentScreenInstruction = 0x21;
            break;
        case 12:
            sub_02039AD8(3);
            break;
        case 3:
        case 11:
            arg0->commsErrorMessage = v6;
        case 0:
        case 1:
        case 13:
            sub_02039AD8(4);
            break;
        case 2:
            sub_020399EC();
            break;
        }
    } else {
        arg0->networkTimeoutCounter = (s32)(arg0->networkTimeoutCounter + 1);
        if (arg0->networkTimeoutCounter == 0xE10) {
            sub_020399EC();
        }
    }
    return 3;
}

typedef struct BaseTransitionEvent {
    u8 baseOwnerNetID;
    u8 success;
    u8 entering;
} BaseTransitionEvent;

int ov70_0224063C(void);

int ov70_0224063C(void) {
    return sizeof(BaseTransitionEvent);
}
