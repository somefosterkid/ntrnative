#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_01_021F8D80.h"
#include "overlay_01_021FD41C.h"
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

struct UnionRoomManager;

struct UnkStruct_0205AC88;

struct UnkStruct_02092BB8;

#if !defined(SDK_PORT) && !defined(NONMATCHING)
struct UnkStruct_ov01_021F8D80;

typedef struct UnkStruct_ov01_021F8D80 {
    u8 filler_00[0x10];
    s8 unk10;
    u8 filler_11[3];
    s8 unk14;
    u8 unk15;
    u8 filler_16;
    u8 unk17_0 : 1;
    u8 unk17_1 : 1;
    u8 unk17_2 : 1;
    u8 unk17_rest : 5;
} UnkStruct_ov01_021F8D80;
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
extern void sub_02023EE0(struct Billboard *billboard, int a1);
#else
extern void sub_02023EE0(Sprite *sprite, int a1);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
extern void sub_02023F04(struct Billboard *billboard, fx32 a1);
#else
extern void sub_02023F04(Sprite *sprite, fx32 a1);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
extern void sub_02023F40(struct Billboard *billboard, int a1);
#else
extern void sub_02023F40(Sprite *sprite, int a1);
#endif

extern int ov01_021FA44C(u32 dir);

extern fx32 ov01_022054E0(LocalMapObject *object);

extern int ov01_02205564(LocalMapObject *object);

void ov01_021F8E70(LocalMapObject *object, u32 direction, VecFx32 *vec);

BOOL ov01_021F8FC0(s32 dir, LocalMapObject *object, void *animObj);

BOOL ov01_021F9344(LocalMapObject *object);

#if defined(SDK_PORT) || defined(NONMATCHING)
#include "overlay_01_021F944C.h"

void ov01_021F8D80(LocalMapObject *object, Billboard *animObj, MapObjectBillboardWorkB *a2, u32 facingDir, BOOL flag);

void ov01_021F8D80(LocalMapObject *object, Billboard *animObj, MapObjectBillboardWorkB *a2, u32 facingDir, BOOL flag) {
    VecFx32 facingVec;
    BOOL paused = ov01_021F9344(object);
    BOOL flag4 = MapObject_CheckFlag4(object);
    int r0 = ov01_02205564(object);
    if ((((u32)(a2->flags << 0x1D)) >> 0x1F) != 0) {
        MapObject_CopyFacingVector(object, &facingVec);
        facingVec.y = a2->unk_14 << 0xc;
    } else {
        if (paused != 0 || flag4 != 0 || r0 != 0) {
            MapObject_CopyFacingVector(object, &facingVec);
            facingVec.x = 0;
            facingVec.z = 0;
        } else {
            facingVec.x = 0;
            facingVec.y = 0;
            facingVec.z = 0;
        }
        if (flag != 0) {
            ov01_021F8E70(object, facingDir, &facingVec);
        }
        facingVec.y += ov01_022054E0(object);
    }
    if (facingDir != (u32)a2->unk_10) {
        sub_02023EE0(animObj, ov01_021FA44C(facingDir));
        sub_02023F40(animObj, 0);
        a2->unk_15 = 0;
    }
    if (paused == 0 && flag4 == 0) {
        sub_02023F04(animObj, 1 << 0xc);
        if (ov01_021F8FC0((u8)facingDir, object, animObj) != 0) {
            facingVec.y -= 2 << 0xc;
            a2->unk_15 = 1;
        } else {
            a2->unk_15 = 0;
        }
    }
    if (flag4 != 0) {
        sub_02023F04(animObj, 1 << 0xc);
    }
    MapObject_SetFacingVector(object, &facingVec);
}
#else
#if !defined(SDK_PORT) && !defined(NONMATCHING)
void ov01_021F8D80(LocalMapObject *object, void *animObj, UnkStruct_ov01_021F8D80 *a2, u32 facingDir, BOOL flag);
#endif

void ov01_021F8D80(LocalMapObject *object, void *animObj, UnkStruct_ov01_021F8D80 *a2, u32 facingDir, BOOL flag) {
    VecFx32 facingVec;
    BOOL paused = ov01_021F9344(object);
    BOOL flag4 = MapObject_CheckFlag4(object);
    int r0 = ov01_02205564(object);
    if (a2->unk17_2 != 0) {
        MapObject_CopyFacingVector(object, &facingVec);
        facingVec.y = a2->unk14 << 0xc;
    } else {
        if (paused != 0 || flag4 != 0 || r0 != 0) {
            MapObject_CopyFacingVector(object, &facingVec);
            facingVec.x = 0;
            facingVec.z = 0;
        } else {
            facingVec.x = 0;
            facingVec.y = 0;
            facingVec.z = 0;
        }
        if (flag != 0) {
            ov01_021F8E70(object, facingDir, &facingVec);
        }
        facingVec.y += ov01_022054E0(object);
    }
    if (facingDir != (u32)a2->unk10) {
        sub_02023EE0(animObj, ov01_021FA44C(facingDir));
        sub_02023F40(animObj, 0);
        a2->unk15 = 0;
    }
    if (paused == 0 && flag4 == 0) {
        sub_02023F04(animObj, 1 << 0xc);
        if (ov01_021F8FC0((u8)facingDir, object, animObj) != 0) {
            facingVec.y -= 2 << 0xc;
            a2->unk15 = 1;
        } else {
            a2->unk15 = 0;
        }
    }
    if (flag4 != 0) {
        sub_02023F04(animObj, 1 << 0xc);
    }
    MapObject_SetFacingVector(object, &facingVec);
}
#endif

extern u8 ov01_021FA298(u32 spriteId);

void ov01_021F8E70(LocalMapObject *object, u32 direction, VecFx32 *vec) {
    VecFx32 facingVec;
    MapObject_CopyFacingVector(object, &facingVec);
    if (ov01_021FA298(MapObject_GetSpriteID(object)) == 0xa) {
        switch (direction) {
        case 0:
            vec->z += 1 << 0xc;
            break;
        case 1:
            vec->z -= 1 << 0xc;
            break;
        case 2:
            vec->x += 0xa << 0xc;
            break;
        case 3:
            vec->x -= 0xa << 0xc;
            break;
        }
    } else {
        switch (direction) {
        case 2:
            vec->x += 2 << 0xc;
            break;
        case 3:
            vec->x -= 2 << 0xc;
            break;
        }
    }
}

struct FieldObjMove;

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

void ov01_021F8F08(LocalMapObject *object, BOOL on);

void ov01_021F8F08(LocalMapObject *object, BOOL on) {
    FieldObjMove *mv = (FieldObjMove *)sub_0205F40C(object);
    if (on) {
        VecFx32 vec = { 0, 0, 0 };
        mv->unk17_2 = 1;
        ov01_021FD9CC(MapObject_GetFacingDirection(object), &vec);
        ((s8 *)mv)[0x12] = vec.x / 0x1000;
        ((s8 *)mv)[0x13] = vec.z / 0x1000;
    } else {
        mv->unk17_2 = 0;
        ((s8 *)mv)[0x12] = 0;
        ((s8 *)mv)[0x13] = 0;
    }
}
