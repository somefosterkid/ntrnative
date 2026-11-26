#include "global.h"

#include "encounter.h"
#include "field_system.h"
#include "heap.h"
#include "launch_application.h"
#include "overlay_13_thumb_1.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "save_vars_flags.h"
#include "sys_flags.h"
#include "task.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_02078834.h"
#include "unk_020968B0.h"

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

struct FrontierLaunchArgs;

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

struct OverlayManager;

struct OverlayManagerTemplate;

struct PhoneBookEntry;

struct PhoneCallPersistentState;

struct PlayerAvatar;

struct PlayerSaveData;

struct PokedexData;

struct PokegearRingingTask;

struct SaveData;

struct SavePokegear;

struct SaveVarsFlags;

struct Signpost;

struct SysTask;

struct SysTaskQueue;

struct TaskManager;

struct TaskManagerUnkSub1C;

struct TerrainAttributes;

struct UnionRoomManager;

struct UnkStruct_0205AC88;

struct UnkStruct_02092BB8;

FS_EXTERN_OVERLAY(OVY_44);

FS_EXTERN_OVERLAY(OVY_91);

FS_EXTERN_OVERLAY(OVY_92);

FS_EXTERN_OVERLAY(OVY_93);

typedef struct {
    SaveData *saveData;
    int unk4;
    int unk8;
} UnkSub0;

typedef struct {
    UnkSub0 *unk0;
    int state;
    u16 *unk8;
    u8 unkC;
    u8 unkD;
    u8 pad_E[2];
    void *unk10;
    int unk14;
} Work;

typedef struct {
    u8 filler0[0x34];
    SaveData *unk34;
    int unk38;
    int unk3c;
} UnkAppArgs40;

extern int ov91_0225C540(OverlayManager *manager, int *state);

extern int ov91_0225C58C(OverlayManager *manager, int *state);

extern int ov91_0225C9EC(OverlayManager *manager, int *state);

extern int ov92_0225CAB4(OverlayManager *manager, int *state);

extern int ov92_0225CDF4(OverlayManager *manager, int *state);

extern int ov92_0225D36C(OverlayManager *manager, int *state);

extern int ov93_0225C540(OverlayManager *manager, int *state);

extern int ov93_0225C574(OverlayManager *manager, int *state);

extern int ov93_0225C6C0(OverlayManager *manager, int *state);

extern int ov44_02232EA8(OverlayManager *manager, int *state);

extern int ov44_02232F64(OverlayManager *manager, int *state);

extern int ov44_022330A8(OverlayManager *manager, int *state);

extern void LoadOVY13(void);

void sub_02078B9C(Work *work, FieldSystem *fieldSystem, enum HeapID heapId, s8 a3);

int sub_02078BD8(Work *work);

#if defined(SDK_PORT) || defined(NONMATCHING)
UnkAppArgs3C *sub_02078C18(FieldSystem *fieldSystem, enum HeapID heapId, s8 a2);
#else
UnkAppArgs3C *sub_02078C18(FieldSystem *fieldSystem, enum HeapID heapId, int a2);
#endif

void sub_02078C60(void *a0);

UnkAppArgs40 *sub_02078C74(FieldSystem *fieldSystem, enum HeapID heapId, int a2);

void sub_02078CB4(void *a0);

#if defined(SDK_PORT) || defined(NONMATCHING)
UnkAppArgs3C *sub_02078CC8(FieldSystem *fieldSystem, enum HeapID heapId, s8 a2);
#else
UnkAppArgs3C *sub_02078CC8(FieldSystem *fieldSystem, enum HeapID heapId, int a2);
#endif

void sub_02078D10(void *a0);

extern const OverlayManagerTemplate _02101260[5];

#if defined(SDK_PORT) || defined(NONMATCHING)
extern int ov44_0222A4B4(OverlayManager *manager, int *state);
extern int ov44_0222A60C(OverlayManager *manager, int *state);
extern int ov44_0222A758(OverlayManager *manager, int *state);

// The asm cuts this table into five labels (_02101260, _02101270, _02101280, _02101290, _021012A0), one record each.
const OverlayManagerTemplate _02101260[5] = {
    {
        .init = ov91_0225C540,
        .exec = ov91_0225C58C,
        .exit = ov91_0225C9EC,
        .ovy_id = FS_OVERLAY_ID(OVY_91),
    },
    {
        .init = ov92_0225CAB4,
        .exec = ov92_0225CDF4,
        .exit = ov92_0225D36C,
        .ovy_id = FS_OVERLAY_ID(OVY_92),
    },
    {
        .init = ov44_02232EA8,
        .exec = ov44_02232F64,
        .exit = ov44_022330A8,
        .ovy_id = FS_OVERLAY_ID(OVY_44),
    },
    {
        .init = ov44_0222A4B4,
        .exec = ov44_0222A60C,
        .exit = ov44_0222A758,
        .ovy_id = FS_OVERLAY_ID(OVY_44),
    },
    {
        .init = ov93_0225C540,
        .exec = ov93_0225C574,
        .exit = ov93_0225C6C0,
        .ovy_id = FS_OVERLAY_ID(OVY_93),
    },
};
#endif

BOOL sub_02078834(TaskManager *taskman);

BOOL sub_02078834(TaskManager *taskman) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskman);
    Work *work = TaskManager_GetEnvironment(taskman);
    switch (work->state) {
    case 0:
        work->unk0->saveData = fieldSystem->saveData;
        // fallthrough
    case 1:
        work->state++;
        if (work->unk0->unk4 == 1 && sub_0203A05C(fieldSystem->saveData)) {
            work->state = 0xA;
            *work->unk8 = 0;
        }
        break;
    case 2:
        CallApplicationAsTask(taskman, &_02101260[3], work->unk0);
        work->state++;
        break;
    case 3:
        if (sub_0203A05C(fieldSystem->saveData)) {
            SetFlag970(Save_VarsFlags_Get(fieldSystem->saveData));
        }
        switch (work->unk0->unk4) {
        case 0:
            break;
        case 3:
            work->unkC = 0;
            work->unkD = 0;
            work->state = 4;
            break;
        case 1:
            work->unkC = 0x32;
            work->unkD = 0;
            work->state = 4;
            break;
        case 2:
            work->unkC = 0x64;
            work->unkD = 0;
            work->state = 4;
            break;
        case 6:
            work->unkC = 0;
            work->unkD = 1;
            work->state = 4;
            break;
        case 4:
            work->unkC = 0x32;
            work->unkD = 1;
            work->state = 4;
            break;
        case 5:
            work->unkC = 0x64;
            work->unkD = 1;
            work->state = 4;
            break;
        case 7:
            work->state = 6;
            break;
        case 10:
            *work->unk8 = 1;
            work->state = 0xB;
            break;
        case 8:
            work->state = 9;
            break;
        case 12:
            work->state = 0xC;
            break;
        case 13:
            work->state = 0xE;
            break;
        case 14:
            work->state = 0x12;
            break;
        case 15:
            work->state = 0x16;
            break;
        case 9:
            work->state = 8;
            break;
        case 11:
            break;
        }
        break;
    case 4:
        CallTask_02050960(taskman, work->unk0->unk8, work->unkC, work->unkD);
        work->state++;
        break;
    case 5:
        work->state = 2;
        break;
    case 6:
        CallTask_WirelessTrade(taskman);
        work->state++;
        break;
    case 7:
        work->state = 2;
        break;
    case 8:
        Heap_Create(HEAP_ID_3, HEAP_ID_53, 0x40100);
        LoadOVY13();
        ov13_0221BA00(HEAP_ID_53);
        OS_ResetSystem(0);
        break;
    case 9:
    case 10:
    case 11:
        Heap_Free(work->unk0);
        Heap_Free(work);
        work->state++;
        return TRUE;
    case 12:
        sub_020378E4(0);
        work->unk10 = sub_020968B0(fieldSystem, NULL);
        work->state++;
        break;
    case 13:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            Heap_Free(work->unk10);
            work->state = 2;
        }
        break;
    case 14:
        sub_02078B9C(work, fieldSystem, HEAP_ID_FIELD2, 1);
        work->state++;
        break;
    case 15:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            work->state = sub_02078BD8(work);
        }
        break;
    case 16:
        work->unk10 = sub_02078C18(fieldSystem, HEAP_ID_FIELD2, work->unk14);
        work->state++;
        break;
    case 17:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            work->state = 2;
            sub_02078C60(work->unk10);
        }
        break;
    case 18:
        sub_02078B9C(work, fieldSystem, HEAP_ID_FIELD2, 2);
        work->state++;
        break;
    case 19:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            work->state = sub_02078BD8(work);
        }
        break;
    case 20:
        work->unk10 = sub_02078C74(fieldSystem, HEAP_ID_FIELD2, work->unk14);
        work->state++;
        break;
    case 21:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            work->state = 2;
            sub_02078CB4(work->unk10);
        }
        break;
    case 22:
        sub_02078B9C(work, fieldSystem, HEAP_ID_FIELD2, 3);
        work->state++;
        break;
    case 23:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            work->state = sub_02078BD8(work);
        }
        break;
    case 24:
        work->unk10 = sub_02078CC8(fieldSystem, HEAP_ID_FIELD2, work->unk14);
        work->state++;
        break;
    case 25:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            work->state = 2;
            sub_02078D10(work->unk10);
        }
        break;
    default:
        return TRUE;
    }
    return FALSE;
}
