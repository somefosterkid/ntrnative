#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "heap.h"
#include "map_matrix.h"
#include "terrain_attributes.h"
#include "unk_02054648.h"
#include "unk_020648EC.h"

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

int sub_02054954(const FieldSystem *fieldSystem, const VecFx32 *objectPosition, const int tileX, const int tileZ, u8 *newObjectHeightSource);

u32 sub_020549F4(FieldSystem *fieldSystem, VecFx32 *playerPos, u32 x, u32 y, u32 *a4) {
    u8 selector;
    int elev = sub_02054954(fieldSystem, playerPos, (int)x, (int)y, &selector);
    if (a4 != NULL) {
        *(u8 *)a4 = (u8)elev;
    }
    if (elev == 0) {
        u32 result;
        if (!Gymmick_CheckCollision(fieldSystem, x, y, (u32)playerPos->y, (BOOL *)&result)) {
            result = sub_020548C0(fieldSystem, (int)x, (int)y);
            if (!result && selector == 2) {
                GetMetatileBehavior(fieldSystem, (int)x, (int)y);
            }
            return result;
        }
        return result;
    }
    return 1;
}
