#include <dwc.h>

#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "pm_string.h"
#include "save.h"
#include "options.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_80_frontier_opponent.h"
#include "overlay_70_gtsapplication_state.h"
#include "message_format.h"
#include "msgdata.h"

struct OverlayManager;

struct OverlayManagerTemplate;

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


typedef u16 charcode_t;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef struct Menu Menu;


typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);


void ov73_021E844C(WifiBattleTowerAppState *appState, int exitMode, int unused);

void WifiBattleTower_SetState(WifiBattleTowerAppState *appState, enum NintendoWfcState state, enum NintendoWfcState nextState);

void ov73_021E8148(WifiBattleTowerAppState *appState, OverlayManager *appMan);

void ov73_021E8148(WifiBattleTowerAppState *appState, OverlayManager *appMan) {
    appState->args = (NintendoWifiConnectArgs *)OverlayManager_GetArgs(appMan);
    appState->screenMode = 0;

    ov73_021E844C(appState, 0, 0);
}

void ov73_021E8164(void);

void ov73_021E8164(void) {
}
