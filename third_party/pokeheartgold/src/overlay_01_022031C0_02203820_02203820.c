#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "field_system.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_01_022031C0.h"
#include "sys_task.h"
#include "unk_020689C8.h"
#include "vram_transfer_manager.h"

struct Field3DModelAnimation;

struct Field3dModel;

struct Field3dObject;

struct FieldSystem;

struct LocalMapObject;

struct MapObjectManager;

struct NARC;

struct SysTask;

struct SysTaskQueue;

struct UnkOv01_022031C0;

struct UnkOv01_022031C0_Data;


struct UnkOv01_022031C0_Work;

BOOL ov01_02203820(OverworldAnimManager *param0, UnkOv01_022031C0_Work *work);

BOOL ov01_02203820(OverworldAnimManager *param0, UnkOv01_022031C0_Work *work) {
    VecFx32 pos;
    UnkOv01_022031C0_Data *src = (struct UnkOv01_022031C0_Data *)(sub_02068D98(param0));
    work->state = 0;
    work->data = *src;
    work->spriteId = MapObject_GetSpriteID(work->data.mapObject);
    work->objectId = MapObject_GetID(work->data.mapObject);
    work->mapId = MapObject_GetMapID(work->data.mapObject);
    pos = work->data.pos;
    sub_02068DA8(param0, &pos);
    Field3dObject_SetPos(&work->data.manager->obj0, &pos);
    Field3dObject_SetActiveFlag(&work->data.manager->obj0, 0);
    return 1;
}
