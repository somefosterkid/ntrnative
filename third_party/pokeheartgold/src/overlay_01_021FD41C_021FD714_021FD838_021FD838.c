#include "global.h"

#include "field_system.h"
#include "gf_rtc.h"
#include "map_object.h"
#include "overlay_01_021FD41C.h"
#include "sys_task_api.h"
#include "unk_020689C8.h"

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

typedef BOOL (*UnkOv01_021FD41C_Cb1)(OverworldAnimManager *, UnkStruct_ov5_021F15B4 *);

typedef void (*UnkOv01_021FD41C_Cb2)(OverworldAnimManager *, UnkStruct_ov5_021F15B4 *);

typedef struct {
    int unk0;
    UnkOv01_021FD41C_Cb1 unk4;
    UnkOv01_021FD41C_Cb1 unk8;
    UnkOv01_021FD41C_Cb2 unkC;
    UnkOv01_021FD41C_Cb2 unk10;
} UnkOv01_021FD41C_Template;

extern BOOL ov01_021F8F88(LocalMapObject *a0);

extern void ov01_021F8FA0(LocalMapObject *a0, VecFx32 *a1);

extern void sub_020699AC(NNSG3dRenderObj *a0, VecFx32 *a1, VecFx32 *a2, MtxFx33 *a3);

BOOL ov01_021FD6C8(OverworldAnimManager *param0, UnkStruct_ov5_021F15B4 *work);

void ov01_021FD718(OverworldAnimManager *param0, UnkStruct_ov5_021F15B4 *work);

void ov01_021FD7D4(OverworldAnimManager *param0, UnkStruct_ov5_021F15B4 *work);

BOOL ov01_021FD784(OverworldAnimManager *param0, UnkStruct_ov5_021F15B4 *work);

void ov01_021FD92C(OverworldAnimManager *param0, UnkStruct_ov5_021F15B4 *work);

void ov01_021FD980(OverworldAnimManager *param0, UnkStruct_ov5_021F15B4 *work);

extern const struct {
    s32 arr_e90[4];
    UnkOv01_021FD41C_Template tmpl_ea0;
    fx32 arr_eb4[5];
    UnkOv01_021FD41C_Template tmpl_ec8;
    UnkOv01_021FD41C_Template tmpl_edc;
    VecFx32 arr_ef0[3];
    VecFx32 arr_f14[3];
    VecFx32 arr_f38[5];
} ov01_02208E90;

void ov01_021FD838(OverworldAnimManager *param0, UnkStruct_ov5_021F15B4 *work);

void ov01_021FD838(OverworldAnimManager *param0, UnkStruct_ov5_021F15B4 *work) {
    VecFx32 translation;
    VecFx32 scale;
    MtxFx33 matrix;
    VecFx32 off;
    LocalMapObject *mapObject;
    NNSG3dRenderObj *renderObj;
    if (work->unk_0C != 0) {
        return;
    }
    *(MtxFx33 *)&matrix = *(const MtxFx33 *)ov01_02208E90.arr_ef0;
    renderObj = &work->unk_14.unk_04->unk_78[0].g3DRenderObj;
    mapObject = work->unk_14.unk_08;
    ov01_021FD488(work->unk_14.unk_04, &scale);
    if (ov01_021F8F88(mapObject) != 0) {
        sub_02068DB8(param0, &translation);
        translation.x -= 2 << 0xa;
        translation.y -= 2 << 0xd;
        translation.z += 2 << 0xb;
        ov01_021FD9CC(MapObject_GetFacingDirection(mapObject), &translation);
    } else {
        MapObject_CopyPositionVector(mapObject, &translation);
        ov01_021F8FA0(mapObject, &off);
        translation.x += off.x;
        translation.z += off.z;
        translation.x -= 2 << 0xa;
        translation.y -= 2 << 0xd;
        translation.z += 2 << 0xb;
    }
    sub_020699AC(renderObj, &translation, &scale, &matrix);
}
