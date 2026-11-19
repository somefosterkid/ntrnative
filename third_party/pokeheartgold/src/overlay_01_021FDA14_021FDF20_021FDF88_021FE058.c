#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_unk_ov01_021fda14_work_a.h"
#include "overworld_anim_manager.h"

void ov01_021FE058(OverworldAnimManager *manager, UnkOv01_021FDA14_WorkA *work) {
    VecFx32 pos;
    LocalMapObject *mapObject;
    s32 scale;

    mapObject = work->data.mapObject;
    if (work->spriteId != MapObject_GetSpriteID(mapObject) || sub_0205F0A8(mapObject, work->objectId, work->mapId) == 0 || MapObject_CheckFlag24(mapObject) == 0) {
        ov01_021F1640(manager);
        return;
    }
    scale = work->scale.x + work->scaleDelta;
    work->scale.x = scale;
    if (scale >= 0x1200) {
        work->scale.x = 0x1200;
        work->scaleDelta = -work->scaleDelta;
    } else if (scale <= 0xE00) {
        work->scale.x = 0xE00;
        work->scaleDelta = -work->scaleDelta;
    }
    ov01_021FDC7C(work, mapObject, &pos);
    pos.y -= 0x514;
    sub_02068DA8(manager, &pos);
    if (work->hasBillboard == 0) {
        ov01_021FE190(manager, work);
    }
}
