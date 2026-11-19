#include "global.h"

#include "constants/sndseq.h"

#include "field_system.h"
#include "heap.h"
#include "map_object.h"
#include "metatile_behavior.h"
#include "overlay_04.h"
#include "player_avatar.h"
#include "task.h"
#include "unk_02005D10.h"

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

typedef struct {
    int unk0;
    int unk4;
    int unk8;
    FieldSystem *unkC;
    PlayerAvatar *unk10;
    int unk14;
} UnkOv01_021F3114;

extern int sub_0206234C(int a0, int a1);

extern void sub_0205DFD4(PlayerAvatar *avatar, int a1);

extern BOOL sub_0205DFC8(PlayerAvatar *avatar);

extern BOOL sub_0205DA34(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction);

int ov01_021F31A0(int direction);

void ov01_021F336C(void *work);

BOOL ov01_021F31CC(TaskManager *taskManager);

BOOL ov01_021F31CC(TaskManager *taskManager) {
    UnkOv01_021F3114 *work = TaskManager_GetEnvironment(taskManager);
    LocalMapObject *mapObject = PlayerAvatar_GetMapObject(work->unk10);
    u8 tile = sub_0205F504(mapObject);
    switch (work->unk8) {
    case 0:
        MapObject_SetFlagsBits(mapObject, (MapObjectFlagBits)(1 << 8));
        work->unk8++;
        break;
    case 1:
        if (sub_0205DFC8(work->unk10) != 0) {
            sub_0205DFD4(work->unk10, sub_0206234C(work->unk0, 0xc));
            PlayerAvatar_SetFacingDirection(work->unk10, work->unk0);
            work->unk8++;
            work->unk4 = 7;
        }
        break;
    case 2:
        if (work->unk4 == 2 || work->unk4 == 4 || work->unk4 == 6) {
            work->unk0 = ov01_021F31A0(work->unk0);
            PlayerAvatar_SetFacingDirection(work->unk10, work->unk0);
        }
        if (--work->unk4 != 0) {
            break;
        }
        if (MetatileBehavior_IsSlideEast(tile) == 1) {
            work->unk0 = 3;
            ViridianGymmick_HandleTileAction(work->unkC, tile);
        } else if (MetatileBehavior_IsSlideWest(tile) == 1) {
            work->unk0 = 2;
            ViridianGymmick_HandleTileAction(work->unkC, tile);
        } else if (MetatileBehavior_IsSlideNorth(tile) == 1) {
            work->unk0 = 0;
            ViridianGymmick_HandleTileAction(work->unkC, tile);
        } else if (MetatileBehavior_IsSlideSouth(tile) == 1) {
            work->unk0 = 1;
            ViridianGymmick_HandleTileAction(work->unkC, tile);
        } else if (MetatileBehavior_StopSliding(tile) == 1) {
            work->unk0 = ov01_021F31A0(work->unk0);
            MapObject_ClearFlagsBits(mapObject, (MapObjectFlagBits)0x80);
            MapObject_ClearFlagsBits(mapObject, (MapObjectFlagBits)(1 << 8));
            PlayerAvatar_SetFacingDirection(work->unk10, work->unk0);
            ov01_021F336C(work);
            StopSE(SEQ_SE_DP_F209, 0);
            return TRUE;
        } else {
            work->unk0 = ov01_021F31A0(work->unk0);
        }
        if (sub_0205DA34(work->unk10, mapObject, work->unk0) == 0) {
            work->unk8 = 1;
            break;
        }
        MapObject_ClearFlagsBits(mapObject, (MapObjectFlagBits)0x80);
        MapObject_ClearFlagsBits(mapObject, (MapObjectFlagBits)(1 << 8));
        PlayerAvatar_SetFacingDirection(work->unk10, work->unk0);
        ov01_021F336C(work);
        StopSE(SEQ_SE_DP_F209, 0);
        return TRUE;
    }
    return FALSE;
}
