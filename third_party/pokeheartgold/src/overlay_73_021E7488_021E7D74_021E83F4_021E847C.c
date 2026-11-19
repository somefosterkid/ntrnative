#include <dwc.h>

#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "pm_string.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
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

void ObjCharTransfer_Destroy(void);

void OamManager_Free(void);

BOOL SpriteList_Delete(SpriteList *collection);

typedef struct Menu Menu;

void ObjPlttTransfer_Destroy(void);

void SpriteTransfer_DeleteCharTransferTask(SpriteResource *resource);

void SpriteTransfer_DeletePlttTransferTask(SpriteResource *resource);

void WifiBattleTower_SetState(WifiBattleTowerAppState *appState, enum NintendoWfcState state, enum NintendoWfcState nextState);

void ov73_021E847C(WifiBattleTowerAppState *appState);

void ov73_021E847C(WifiBattleTowerAppState *appState) {
    SpriteTransfer_DeleteCharTransferTask(appState->spriteResource[0][0]);
    SpriteTransfer_DeletePlttTransferTask(appState->spriteResource[0][1]);

    for (int i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(appState->resourceCollection[i]);
    }

    SpriteList_Delete(appState->spriteList);

    appState->spriteList = NULL;

    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
}
