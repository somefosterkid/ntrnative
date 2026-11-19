#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_01_021F8D80.h"
#include "overlay_01_021F944C.h"
#include "sprite.h"

struct AreaDataManager;

struct AreaLightManager;

struct BagCursor;

struct BgConfig;

struct BugContest;

struct Camera;

struct DynamicTerrainHeightManager;

struct Field3dRenderObjManager;

struct FieldEffectManager;

struct FieldObjMove;

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

struct SpriteList;

struct SysTask;

struct SysTaskQueue;

struct TaskManager;

struct TaskManagerUnkSub1C;

struct TerrainAttributes;

struct UnionRoomManager;

struct UnkStruct_0205AC88;

struct UnkStruct_02092BB8;

typedef struct FieldObjMove {
    Sprite *unk00;
    VecFx32 unk04;
    u32 unk10;
    s8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17_0 : 1;
    u8 unk17_1 : 1;
    u8 unk17_2 : 1;
    u8 unk17_rest : 5;
    u8 unk18[8];
} FieldObjMove;

void ov01_021F9154(LocalMapObject *object, Sprite *sprite, FieldObjMove *mv);

void ov01_021F917C(LocalMapObject *object, Sprite *sprite, FieldObjMove *mv);

void ov01_021F91A4(LocalMapObject *object, Sprite *sprite);

extern void (*const ov01_02208B5C[2])(LocalMapObject *, Sprite *, FieldObjMove *);

void ov01_021F9078(LocalMapObject *object);

void ov01_021F9078(LocalMapObject *object) {
    FieldObjMove *mv = (FieldObjMove *)sub_0205F40C(object);
    Sprite *spr = mv->unk00;
    if (ov01_021FA2D4(object) != 1 && spr != NULL) {
        ov01_02208B5C[sub_0205F330(object)](object, spr, mv);
        mv->unk10 = sub_0205F330(object);
        ov01_021FA3E8(object, spr);
        ov01_021F91A4(object, spr);
    }
}
