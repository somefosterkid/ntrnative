#include "global.h"

#include "map_object.h"
#include "player_avatar.h"
#include "overlay_01_unk_ov01_021fda14_work_a.h"
#include "overworld_anim_manager.h"
#include "unk_020689C8.h"

BOOL ov01_021FDFD4(OverworldAnimManager *param0, UnkOv01_021FDA14_WorkA *work) {
    VecFx32 pos;

    work->data = *(const UnkOv01_021FDA14_DataA *)sub_02068D98(param0);
    work->type = sub_02068D90(param0);
    work->objectId = MapObject_GetID(work->data.mapObject);
    work->mapId = MapObject_GetMapID(work->data.mapObject);
    work->spriteId = MapObject_GetSpriteID(work->data.mapObject);
    work->scale.x = FX32_ONE;
    work->scale.y = FX32_ONE;
    work->scale.z = FX32_ONE;
    work->scaleDelta = 0x40;
    if (work->type == 2 || work->type == 5) {
        work->scaleDelta = 0;
    }
    ov01_021FDC7C(work, work->data.mapObject, &pos);
    pos.y -= 0x514;
    sub_02068DA8(param0, &pos);
    ov01_021FE190(param0, work);
    return TRUE;
}
