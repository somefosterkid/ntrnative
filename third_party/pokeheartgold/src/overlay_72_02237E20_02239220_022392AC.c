#include "global.h"
#include "sprite_resources_header.h"
#include "pm_string.h"
#include "save.h"
#include "options.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_80_frontier_opponent.h"
#include "overlay_70_gtsapplication_state.h"
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

struct tagDWCWDSData;

BOOL DWC_NASLoginAsync(void);


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

int ov72_022392AC(WifiBattleTowerAppState *appState);

int ov72_022392AC(WifiBattleTowerAppState *appState) {
    DWC_NASLoginAsync();
    appState->state = NINTENDO_WFC_STATE_WAIT_AUTH;

    return BT_LOOP_STATE_MAIN;
}
