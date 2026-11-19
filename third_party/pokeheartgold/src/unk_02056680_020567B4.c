#include "global.h"

#include "field/fieldmap.h"

#include "assert.h"
#include "camera.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "metatile_behavior.h"
#include "overlay_01.h"
#include "overlay_01_021E90C0.h"
#include "overlay_01_021F1AFC.h"
#include "overlay_manager.h"
#include "player_avatar.h"
#include "screen_fade.h"
#include "task.h"
#include "unk_02054648.h"
#include "unk_02055BF0.h"
#include "unk_02056680.h"
#include "unk_02062108.h"

struct AreaDataManager;

struct AreaLightManager;

struct BagCursor;

struct BgConfig;

struct BugContest;

struct Camera;

struct DynamicTerrainHeightManager;

struct Field3dRenderObjManager;

struct FieldEffectManager;

struct FieldEnvSubUnk18;

struct FieldProcessManager;

struct FieldSystem;

struct FieldSystemUnkSub104;

struct FieldSystemUnkSub108;

struct FieldSystemUnkSub120;

struct FieldSystemUnkSub4;

struct FieldSystemUnkSubC8;

struct FieldTransitionEnvironment;

struct FieldViewPhoto;

struct FogData;

struct FollowMon;

struct FrontierFieldSystem;

struct GearPhoneRingManager;

struct LinkBattleRuleset;

struct LocalMapObject;

struct Location;

struct MapEvents;

struct MapLoadManager;

struct MapLoadMode;

struct MapMatrix;

struct MapObjectManager;

struct MapObjectsToPreload;

struct MapPropAnimationManager;

struct MapPropManager;

struct MapPropOneShotAnimationManager;

struct MenuInputStateMgr;

struct ModelAttributes;

struct NARC;

struct PhoneBookEntry;

struct PhoneCallPersistentState;

struct PlayerAvatar;

struct PlayerSaveData;

struct PokedexData;

struct PokegearRingingTask;

struct SaveData;

struct SavePokegear;

struct Signpost;

struct SysTask;

struct SysTaskQueue;

struct TaskManager;

struct TaskManagerUnkSub1C;

struct TerrainAttributes;

struct UnionRoomManager;

struct UnkStruct_0205AC88;

struct UnkStruct_02092BB8;

BOOL ov01_021E9C40(TaskManager *taskMan);

BOOL sub_020567B4(TaskManager *man) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(man);
    FieldTransitionEnvironment *fenv = TaskManager_GetEnvironment(man);
    LocalMapObject *obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
    switch (fenv->transitionState) {
    case 0: {
        FieldEnvSubUnk18 *unk = fenv->unk18 = ov01_021E90C0();
        ov01_021E90DC(PlayerAvatar_GetXCoord(fieldSystem->playerAvatar), PlayerAvatar_GetZCoord(fieldSystem->playerAvatar), unk);
        TaskManager_Call(man, ov01_021E9C40, unk);
        fenv->transitionState++;
        break;
    }
    case 1:
        ov01_021E90D4(fenv->unk18);
        return TRUE;
    }
    return FALSE;
}
