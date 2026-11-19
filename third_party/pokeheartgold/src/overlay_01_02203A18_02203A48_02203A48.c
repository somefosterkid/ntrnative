#include "global.h"

#include "constants/sndseq.h"

#include "field_system.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_01_02203A18.h"
#include "overlay_01_021F1348.h"
#include "sprite.h"
#include "task.h"
#include "unk_02005D10.h"
#include "vram_transfer_manager.h"

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

struct Sprite;

struct SysTask;

struct SysTaskQueue;

struct TaskManager;

struct TaskManagerUnkSub1C;

struct TerrainAttributes;

struct UnkOv01_02203A18;

struct UnionRoomManager;

struct UnkStruct_0205AC88;

struct UnkStruct_02092BB8;

BOOL ov01_02203BB4(void *param0, UnkOv01_02203A18_Work *work);

void ov01_02203CA0(void *param0, UnkOv01_02203A18_Work *work);

void ov01_02203CB8(void *param0, UnkOv01_02203A18_Work *work);

void ov01_02203DC0(void *param0, UnkOv01_02203A18_Work *work);

extern const OverworldAnimManagerFuncs ov01_02209518;

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_01_02203A18.s (same label), with the context size from the type, not the DS literal.
const OverworldAnimManagerFuncs ov01_02209518 = OVERWORLD_ANIM_MANAGER_FUNCS(UnkOv01_02203A18_Work, ov01_02203BB4, ov01_02203CA0, ov01_02203CB8, ov01_02203DC0);
#endif

void *ov01_02203A48(LocalMapObject *mapObject, int a1);

void *ov01_02203A48(LocalMapObject *mapObject, int a1) {
    UnkOv01_02203A18_Data data;
    VecFx32 pos;
    VecFx32 facing;
    FieldEffectManager *fieldEffMan = ov01_021F146C(mapObject);
    UnkOv01_02203A18 *manager;
    int priority;
    data.fieldEffMan = fieldEffMan;
    manager = ov01_021F1450(fieldEffMan, 0x12);
    data.unk4 = manager;
    data.unk8 = mapObject;
    if (manager->busy != 0) {
        return NULL;
    }
    manager->busy = 1;
    MapObject_CopyPositionVector(mapObject, &pos);
    MapObject_CopyFacingVector(mapObject, &facing);
    facing.y = 0;
    VEC_Add(&pos, &facing, &pos);
    priority = MapObject_GetPriority(mapObject) + 1;
    return ov01_021F1620(fieldEffMan, &ov01_02209518, &pos, a1, &data, priority);
}
