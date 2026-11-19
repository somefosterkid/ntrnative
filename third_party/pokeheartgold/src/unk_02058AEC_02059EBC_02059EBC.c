#include "global.h"

#include "field_move.h"
#include "field_system.h"
#include "heap.h"
#include "mail_message.h"
#include "player_data.h"
#include "sys_task_api.h"
#include "task.h"
#include "unk_02034354.h"
#include "unk_02034B0C.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_02058AEC.h"
#include "unk_0205A44C.h"
#include "unk_02091564.h"
#include "union_room_manager.h"

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

struct PlayerProfile;

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

struct UnkStruct_02059E1C_Sub80;

struct UnkStruct_0205AC88;

struct UnkStruct_02092BB8;

struct UnkStruct_021D41CC;

typedef void (*UnkFuncPtr_02059E1C)(struct UnionRoomManager *param0);

typedef struct UnkStruct_02059E1C_Sub80 UnkStruct_02059E1C_Sub80;

typedef struct UnkStruct_021D41CC {
    int unk0;
    UnkStruct_02059E1C_Sub80 *unk4;
    int unk8;
} UnkStruct_021D41CC;

extern UnkStruct_021D41CC _021D41CC;

extern void sub_0208F814(void *a0);

void sub_02059F78(struct UnionRoomManager *param0);

void sub_0205A034(struct UnionRoomManager *param0, UnkFuncPtr_02059E1C param1, int param2);

void sub_0205A0B4(struct UnionRoomManager *param0);

typedef void (*UnkIntFunc)(int a0);

void sub_02059EBC(struct UnionRoomManager *param0);

void sub_02059EBC(struct UnionRoomManager *param0) {
    if (sub_02037FCC()) {
        _021D41CC.unk0 = 0;
        sub_0205A034(param0, sub_02059F78, 0);
        return;
    }

    if (param0->pendingConnection != 0) {
        param0->unused_28 = 2;

        if (param0->pendingConnection == 1) {
            if (param0->peerActivity == 5) {
                ((UnkIntFunc)sub_0203894C)(param0->connectionID);
            } else if (param0->peerActivity == 6) {
                ((UnkIntFunc)sub_0203898C)(param0->connectionID);
            } else {
                ((UnkIntFunc)sub_02037F64)(param0->connectionID);
            }
        } else if (param0->pendingConnection == 2) {
            sub_0208F814(NULL);
            ((UnkIntFunc)sub_02038918)(param0->connectionID);
        }

        sub_0205A034(param0, sub_0205A0B4, 12);
        return;
    }
}
