#include "global.h"

#include "constants/sndseq.h"

#include "field/overlay_01_021FB878.h"

#include "field_system.h"
#include "heap.h"
#include "overlay_01_022051EC.h"
#include "unk_02005D10.h"
#include "unk_020689C8.h"

struct AreaDataManager;

struct AreaLightManager;

struct BagCursor;

struct BgConfig;

struct BugContest;

struct Camera;

struct DynamicTerrainHeightManager;

struct Field3DModelAnimation;

struct Field3dModel;

struct Field3dObject;

struct Field3dRenderObjManager;

struct FieldEffectManager;

struct FieldProcessManager;

struct FieldSystem;

struct FieldSystemUnkSub104;

struct FieldSystemUnkSub108;

struct FieldSystemUnkSub120;

struct FieldSystemUnkSub4;

struct FieldSystemUnkSubC8;

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

BOOL ov01_02205300(void *param0, UnkOv01_022051EC_Work *work) {
    VecFx32 vec;
    UnkOv01_022051EC_Data *data = (UnkOv01_022051EC_Data *)sub_02068D98(param0);
    UnkOv01_022051EC *manager;
    work->ctx = data->ctx;
    work->state = 0;
    manager = data->ctx;
    ov01_022053C4(&manager->anim[0], 2, 0);
    Field3dObject_SetActiveFlag(&manager->obj, TRUE);
    sub_02068DB8(param0, &vec);
    Field3dObject_SetPosEx(&manager->obj, vec.x, vec.y, vec.z);
    PlaySE(SEQ_SE_GS_TUREARUKI);
    return TRUE;
}
