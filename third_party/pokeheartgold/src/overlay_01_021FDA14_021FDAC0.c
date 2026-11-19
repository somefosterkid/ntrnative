#include "global.h"

#include "map_object.h"
#include "overlay_01_unk_ov01_021fda14_work_a.h"
#include "overworld_anim_manager.h"

BOOL ov01_021FDAC0(OverworldAnimManager *manager, UnkOv01_021FDA14_WorkA *work) {
    VecFx32 pos;

    work->data = *(const UnkOv01_021FDA14_DataA *)sub_02068D98(manager);
    work->type = sub_02068D90(manager);
    work->objectId = MapObject_GetID(work->data.mapObject);
    work->mapId = MapObject_GetMapID(work->data.mapObject);
    work->spriteId = MapObject_GetSpriteID(work->data.mapObject);
    work->scale.x = FX32_ONE;
    work->scale.y = FX32_ONE;
    work->scale.z = FX32_ONE;
    work->scaleDelta = FX32_ONE / 64;
    if (work->type == 2) {
        work->scaleDelta = 0;
    }
    ov01_021FDC7C(work, work->data.mapObject, &pos);
    sub_02068DA8(manager, &pos);
    ov01_021FDD48(manager, work);
    return TRUE;
}
