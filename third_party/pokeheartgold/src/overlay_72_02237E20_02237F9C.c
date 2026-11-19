#include "global.h"

#include "overlay_manager.h"
#include "pm_string.h"
#include "save.h"
#include "options.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_80_frontier_opponent.h"
#include "bg_window.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "message_format.h"
#include "unk_0202D230.h"
#include "sav_system_info.h"
#include "msgdata.h"

struct BgConfig;

struct BoxPokemon;

struct DWCstAccLoginId;

struct DWCstAccUserData;

struct DWCstInetControl;

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

struct SysTaskManager;

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

struct tagDWCWDSData;

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


typedef struct Menu Menu;

enum WifiBattleTowerLoopState {
    BT_LOOP_STATE_WAIT_FOR_WIRELESS_DRIVER,
    BT_LOOP_STATE_INIT,
    BT_LOOP_STATE_WAIT_FADE,
    BT_LOOP_STATE_MAIN,
    BT_LOOP_STATE_FINISH,
    BT_LOOP_STATE_EXIT,
};

void ov72_022378DC(void);

int ov73_021E84D0(WifiBattleTowerAppState *appState, int unused);

int WifiBattleTower_ScreenMain(WifiBattleTowerAppState *appState, int unused);

int WifiBattleTower_ScreenExit(WifiBattleTowerAppState *appState, int unused);

BOOL IsPaletteFadeFinished(void);

BOOL sub_02034DB8(void);

void ov72_022387A0(DWCAllocType sLotoId, void *ptr, u32 unused2);

void *ov72_02238778(DWCAllocType unused, u32 size, int alignment);

extern NNSFndHeapHandle ov72_0223B92C;

extern int (*ov72_0223B654[1][3])(WifiBattleTowerAppState *, int);

#if defined(SDK_PORT) || defined(NONMATCHING)
int ov72_0223886C(WifiBattleTowerAppState *appState, int unused);

int ov72_0223894C(WifiBattleTowerAppState *appState, int unused);

int ov72_0223897C(WifiBattleTowerAppState *appState, int unused);

// The asm cuts this table into one label per entry (ov72_0223B654, ov72_0223B658, ov72_0223B65C).
int (*ov72_0223B654[1][3])(WifiBattleTowerAppState *, int) = {
    { ov72_0223886C, ov72_0223894C, ov72_0223897C },
};
#endif

void WifiBattleTower_SetState(WifiBattleTowerAppState *appState, enum NintendoWfcState state, enum NintendoWfcState nextState);

BOOL NintendoWfc_Main(OverlayManager *appMan, int *state);

BOOL NintendoWfc_Main(OverlayManager *appMan, int *state) {
    WifiBattleTowerAppState *appState = OverlayManager_GetData(appMan);

    DWC_UpdateConnection();
    ov72_022378DC();
    DWC_UpdateConnection();

    switch (*state) {
    case BT_LOOP_STATE_WAIT_FOR_WIRELESS_DRIVER:
        if (sub_02034DB8()) {
            ov72_0223B92C = appState->dwcHeap;
            DWC_SetMemFunc(ov72_02238778, ov72_022387A0);
            *state = BT_LOOP_STATE_INIT;
        }
        break;
    case BT_LOOP_STATE_INIT:
        *state = (*ov72_0223B654[appState->screenMode][0])(appState, *state);
        break;
    case BT_LOOP_STATE_WAIT_FADE:
        if (IsPaletteFadeFinished()) {
            *state = BT_LOOP_STATE_MAIN;
        }
        break;
    case BT_LOOP_STATE_MAIN:
        *state = (*ov72_0223B654[appState->screenMode][1])(appState, *state);
        break;
    case BT_LOOP_STATE_FINISH:
        if (IsPaletteFadeFinished()) {
            *state = (*ov72_0223B654[appState->screenMode][2])(appState, *state);
        }
        break;
    case BT_LOOP_STATE_EXIT:
        return TRUE;
        break;
    }

    if (appState->spriteList != NULL) {
        SpriteList_RenderAndAnimateSprites(appState->spriteList);
    }

    return FALSE;
}
