#include "global.h"

#include "field_system.h"
#include "heap.h"
#include "launch_application.h"
#include "task.h"
#include "unk_02095DF4.h"

struct AreaDataManager;

struct AreaLightManager;

struct BagCursor;

struct BgConfig;

struct BugContest;

struct Camera;

struct DynamicTerrainHeightManager;

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

struct PokeathlonCourseArgs;

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

struct UnkStruct_02095DF4;

struct UnkStruct_02095E30;

BOOL sub_02095E30(TaskManager *taskman);

BOOL sub_02095E30(TaskManager *taskman) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskman);
    UnkStruct_02095DF4 *work = TaskManager_GetEnvironment(taskman);
    switch (work->unk00) {
    case 0:
        work->unk04 = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(UnkStruct_02095E30));
        work->unk04->mode = (work->unk08 != 0);
        work->unk04->saveData = FieldSystem_GetSaveData(fieldSystem);
        work->unk04->unk0C = work->unk0C;
        work->unk04->unk0F = work->unk0E;
        PokeathlonCourse_LaunchApp(fieldSystem, (PokeathlonCourseArgs *)work->unk04);
        sub_0203E30C();
        work->unk00 = 1;
        break;
    case 1:
        if (FieldSystem_ApplicationIsRunning(fieldSystem)) {
            break;
        }
        work->unk00 = 2;
        FieldSystem_LoadFieldOverlay(fieldSystem);
        break;
    case 2:
        if (!sub_020505C8(fieldSystem)) {
            break;
        }
        work->unk00 = 3;
        break;
    case 3:
        *work->unk10 = work->unk04->unk0E;
        *work->unk14 = work->unk04->unk0A;
        *work->unk18 = work->unk04->unk08;
        *work->unk1C = work->unk04->unk0D;
        Heap_Free(work->unk04);
        Heap_Free(work);
        return TRUE;
    }
    return FALSE;
}
