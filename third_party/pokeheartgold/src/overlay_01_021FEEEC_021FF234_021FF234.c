#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FEEEC.h"
#include "overworld_anim_manager.h"
#include "unk_0205FD20.h"
#include "unk_020689C8.h"

void sub_02023EA4(Billboard *billboard, u8 draw);
void sub_02023F04(Billboard *billboard, fx32 numFrames);
void sub_02023F1C(Billboard *billboard, fx32 frameNum);
fx32 sub_02023F70(const Billboard *billboard);

void ov01_021FF234(OverworldAnimManager *manager, FieldEffectAnimWork *work) {
    VecFx32 curPos;
    VecFx32 tilePos;
    LocalMapObject *mapObject;
    s32 heightSet;
    s32 frame;
    s32 dz;
    s32 dx;

    mapObject = work->args.mapObject;
    if (sub_0205F0F8(mapObject, work->spriteId, work->objectId, work->mapId) == 0) {
        ov01_021F1640(manager);
        return;
    }
    if (MapObject_TestFlagsBits(mapObject, MAPOBJECTFLAG_VISIBLE) == 1) {
        sub_02023EA4(work->billboard, 0);
    } else {
        sub_02023EA4(work->billboard, 1);
    }
    if (work->heightSet == 0) {
        sub_02068DB8(manager, &curPos);
        tilePos.x = work->args.pos.x << 0x10;
        tilePos.z = work->args.pos.z << 0x10;
        tilePos.y = curPos.y;
        heightSet = sub_0206121C(work->args.fieldSystem, &tilePos);
        work->heightSet = heightSet;
        if (heightSet == 1) {
            curPos.y = tilePos.y;
            sub_02068DA8(manager, &curPos);
        }
    }
    switch (work->state) {
    case 0:
        sub_02023F04(work->billboard, 0x1000);
        frame = sub_02023F70(work->billboard);
        if (frame / 0x1000 >= 0xC) {
            work->state = 1;
            return;
        }
        return;

    case 1:
        sub_02023F1C(work->billboard, 0xC000);
        work->state = 2;

    case 2:
        if (sub_0205F0F8(mapObject, work->spriteId, work->objectId, work->mapId) == 0) {
            ov01_021F1640(manager);
            return;
        }
        dx = MapObject_GetXCoord(mapObject) - work->args.unk_1C;
        dz = MapObject_GetZCoord(mapObject) - work->args.unk_1E;
        if (work->args.pos.x != dx || work->args.pos.z != dz) {
            ov01_021F1640(manager);
            return;
        }
        if (work->args.direction != -1 && work->args.direction != MapObject_GetFacingDirection(mapObject)) {
            ov01_021F1640(manager);
        }
        break;
    }
}
