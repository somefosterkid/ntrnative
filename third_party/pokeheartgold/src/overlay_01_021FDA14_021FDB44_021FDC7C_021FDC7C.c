#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "map_object.h"
#include "player_avatar.h"
#include "sys_task.h"
#include "unk_0205FD20.h"
#include "overlay_01_unk_ov01_021fda14_work_a.h"

struct FieldSystem;

struct LocalMapObject;

struct MapObjectManager;

struct NARC;

struct PlayerAvatar;

struct PlayerSaveData;

struct SysTask;

struct SysTaskQueue;

struct UnkOv01_021FDA14_BillboardResources;

struct UnkOv01_021FDA14_DataB;

struct UnkOv01_021FDA14_Offsets;

struct UnkOv01_021FDA14_Template;

struct UnkOv01_021FDA14_WorkB;

typedef struct UnkOv01_021FDA14_BillboardResources {
    void *modelRes;
    u8 unk4[0x24];
} UnkOv01_021FDA14_BillboardResources;

typedef struct UnkOv01_021FDA14_DataB {
    FieldSystem *fieldSystem;
    void *fxMgr;
    void *manager;
    UnkOv01_021FDA14_BillboardResources res;
    void *srcBillboard;
} UnkOv01_021FDA14_DataB;

typedef struct UnkOv01_021FDA14_WorkB {
    u32 type;
    UnkOv01_021FDA14_DataB data;
    void *billboard;
    VecFx32 scale;
    fx32 scaleDelta;
    VecFx32 basePos;
} UnkOv01_021FDA14_WorkB;

typedef struct UnkOv01_021FDA14_Template {
    u32 workSize;
    BOOL (*init)
    (void *, void *);
    BOOL (*delete)
    (void *, void *);
    void (*update)(void *, void *);
    void (*draw)(void *, void *);
} UnkOv01_021FDA14_Template;

typedef struct UnkOv01_021FDA14_Offsets {
    fx32 offsets[6];
} UnkOv01_021FDA14_Offsets;

BOOL ov01_021FDEAC(void *param0, UnkOv01_021FDA14_WorkB *work);

BOOL ov01_021FDF14(void *param0, UnkOv01_021FDA14_WorkB *work);

void ov01_021FDF20(void *param0, UnkOv01_021FDA14_WorkB *work);

void ov01_021FDF64(void *param0, UnkOv01_021FDA14_WorkB *work);

extern const struct {
    UnkOv01_021FDA14_Template template_02208F74;
    UnkOv01_021FDA14_Template template_02208F88;
    UnkOv01_021FDA14_Template template_02208F9C;
    UnkOv01_021FDA14_Offsets offsets_02208FB0;
    UnkOv01_021FDA14_Offsets offsets_02208FC8;
} sRodata;

extern const UnkOv01_021FDA14_Offsets ov01_02208FC8;

void ov01_021FDC7C(UnkOv01_021FDA14_WorkA *work, LocalMapObject *mapObject, VecFx32 *out) {
    VecFx32 facing;
    UnkOv01_021FDA14_Offsets offsets = ov01_02208FC8;
    fx32 x;
    fx32 yOffset;
    fx32 z;
    int result;

    MapObject_CopyFacingVector(mapObject, &facing);
    x = facing.x;
    if (MapObject_GetID(mapObject) == 0xFF && MapObject_GetSpriteID(mapObject) == 0xBC) {
        int dir = MapObject_GetFacingDirection(mapObject);
        z = 0x800;
        if (dir == 1) {
            z *= 3;
            z -= facing.z;
        } else {
            z = facing.z;
        }
    } else {
        z = facing.z;
    }
    yOffset = -(facing.y / 6);
    if (MapObject_GetID(mapObject) == 0xFD && MapObject_GetSpriteID(PlayerAvatar_GetMapObject(work->data.fieldSystem->playerAvatar)) == 0xBC) {
        yOffset = 0;
    }
    MapObject_CopyPositionVector(mapObject, out);
    result = sub_0206121C(work->data.fieldSystem, out);
    out->x += x;
    out->z += z - ((fx32)(((7) > 0) ? ((7) * (1 << 12) + 0.5f) : ((7) * (1 << 12) - 0.5f)));
    if (result == 0) {
        out->y = 0;
    } else {
        out->y -= offsets.offsets[work->type];
    }
    out->y += yOffset;
}
