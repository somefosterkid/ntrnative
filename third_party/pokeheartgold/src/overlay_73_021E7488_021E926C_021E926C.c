#include <dwc.h>

#include "global.h"
#include "sprite_resources_header.h"

#include "unk_02005D10.h"
#include "pm_string.h"
#include "save.h"
#include "options.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_80_frontier_opponent.h"
#include "unk_0202D230.h"
#include "screen_fade_internal.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "system.h"
#include "message_format.h"
#include "msgdata.h"

struct BgConfig;

struct BoxPokemon;

struct FrontierPokemon;

struct G2dRenderer;

struct Menu;

struct MsgData;

struct Options;

struct SaveData;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskQueue;

struct System;

struct SysInfo;

struct WifiBattleTowerAppState;

struct WifiBattleTowerDownloadBuffer;

struct WifiBattleTowerDownloadData;

struct WifiBattleTowerMatchCandidate;

struct WifiBattleTowerRecord;

struct WifiPlayerProfile;

struct Window;

struct WorldExchangeTrainer;

struct WorldExchangeTrainerError;

BOOL DWC_GetDateTime(RTCDate *date, RTCTime *time);




typedef u16 charcode_t;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef struct Menu Menu;

enum WifiBattleTowerLoopState {
    BT_LOOP_STATE_WAIT_FOR_WIRELESS_DRIVER,
    BT_LOOP_STATE_INIT,
    BT_LOOP_STATE_WAIT_FADE,
    BT_LOOP_STATE_MAIN,
    BT_LOOP_STATE_FINISH,
    BT_LOOP_STATE_EXIT,
};

BOOL sub_0202D720(WifiBattleTowerDownloadData *downloadData, u8 rank, u8 opponentIdx, RTCDate *date);

typedef void (*UnkCallback)(void);

enum ButtonMode {
    BUTTON_MODE_NORMAL = 0,
    BUTTON_MODE_START_IS_X = 1,
    BUTTON_MODE_SWAP_XY = 2,
    BUTTON_MODE_L_IS_A = 3,
};

void ov73_021EA0F0(Window *window, String *string, int num, int maxDigits);

void ov73_021EA134(WifiBattleTowerAppState *appState, int selectionMode, int visible);

int ov73_021E926C(WifiBattleTowerAppState *appState);

int ov73_021E926C(WifiBattleTowerAppState *appState) {
    if (gSystem.newAndRepeatedKeys & 0x0040) {
        appState->selectedOpponentIdx++;

        if (appState->selectedOpponentIdx > appState->matchCount) {
            appState->selectedOpponentIdx = 1;
        }

        ov73_021EA0F0(&appState->opponentWindow, appState->opponentIdxString, appState->selectedOpponentIdx, 3);
        PlaySE(1500);
    } else if (gSystem.newAndRepeatedKeys & 0x0080) {
        appState->selectedOpponentIdx--;

        if (appState->selectedOpponentIdx < 1) {
            appState->selectedOpponentIdx = appState->matchCount;
        }

        ov73_021EA0F0(&appState->opponentWindow, appState->opponentIdxString, appState->selectedOpponentIdx, 3);
        PlaySE(1500);
    } else if (gSystem.newKeys & 0x0001) {
        {
            RTCTime time;
            RTCDate date;

            DWC_GetDateTime(&date, &time);

            if (sub_0202D720(appState->args->downloadData, appState->selectedRank, appState->selectedOpponentIdx, &date)) {
                appState->state = NINTENDO_WFC_STATE_PRINT_ROOM_UNAVAILABLE;
            } else {
                appState->state = NINTENDO_WFC_STATE_PRINT_RECEIVING_TRAINERS;
            }
        }

        ov73_021EA134(appState, 0, 0);
        PlaySE(1500);
    } else if (gSystem.newKeys & 0x0002) {
        ov73_021EA134(appState, 0, 0);
        appState->state = NINTENDO_WFC_STATE_ASK_CANCEL_CHALLENGE;
        PlaySE(1500);
    }

    return BT_LOOP_STATE_MAIN;
}
