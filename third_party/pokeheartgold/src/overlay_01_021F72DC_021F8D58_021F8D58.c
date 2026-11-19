#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "overlay_01_021FD2EC.h"
#include "overworld_anim_manager.h"
#include "sys_task.h"

typedef struct {
    OverworldAnimManager *unk_00;
} UnkStruct_ov5_021EC760;

void ov01_021F8CC8(u32 param0, VecFx32 *param1);

void ov01_021F8D58(LocalMapObject *mapObj);

void ov01_021F8D58(LocalMapObject *mapObj) {
    VecFx32 v0;
    UnkStruct_ov5_021EC760 *v1 = (UnkStruct_ov5_021EC760 *)sub_0205F40C(mapObj);
    ov01_021F8CC8(MapObject_GetSpriteID(mapObj), &v0);
    v1->unk_00 = ov01_021FD2EC(mapObj, &v0);
}
