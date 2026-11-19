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

void ov01_021F8CFC(LocalMapObject *mapObj);

void ov01_021F8D24(void);

void ov01_021F1640(OverworldAnimManager *animMan);
void ov01_021F8D28(LocalMapObject *mapObj);
void ov01_021F8D40(LocalMapObject *mapObj);

void ov01_021F8CFC(LocalMapObject *mapObj) {
    VecFx32 v0;
    UnkStruct_ov5_021EC760 *v1 = (UnkStruct_ov5_021EC760 *)sub_0205F3E8(mapObj, (sizeof(UnkStruct_ov5_021EC760)));
    ov01_021F8CC8(MapObject_GetSpriteID(mapObj), &v0);
    v1->unk_00 = ov01_021FD2EC(mapObj, &v0);
}

void ov01_021F8D24(void) {
}

void ov01_021F8D28(LocalMapObject *mapObj) {
    UnkStruct_ov5_021EC760 *v0 = (UnkStruct_ov5_021EC760 *)sub_0205F40C(mapObj);

    if (v0->unk_00 != 0L) {
        ov01_021F1640(v0->unk_00);
        v0->unk_00 = 0L;
    }
}

void ov01_021F8D40(LocalMapObject *mapObj) {
    UnkStruct_ov5_021EC760 *v0 = (UnkStruct_ov5_021EC760 *)sub_0205F40C(mapObj);

    if (v0->unk_00 != 0L) {
        ov01_021F1640(v0->unk_00);
        v0->unk_00 = 0L;
    }
}
