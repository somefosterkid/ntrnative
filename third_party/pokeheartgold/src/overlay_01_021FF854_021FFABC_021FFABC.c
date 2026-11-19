#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FF854.h"
#include "overworld_anim_manager.h"
#include "unk_0205FD20.h"
#include "unk_020689C8.h"

void sub_02023EA4(Billboard *billboard, u8 draw);
void sub_02023F04(Billboard *billboard, fx32 numFrames);
void sub_02023F1C(Billboard *billboard, fx32 frameNum);
fx32 sub_02023F70(const Billboard *billboard);

void ov01_021FFABC(OverworldAnimManager *manager, FieldEffectAnimWork *work) {
    LocalMapObject *obj = work->args.mapObject;
    if (sub_0205F0F8(obj, work->spriteId, work->objectId, work->mapId) == 0) {
        ov01_021F1640(manager);
        return;
    }
    if (MapObject_TestFlagsBits(obj, MAPOBJECTFLAG_VISIBLE) == 1) {
        sub_02023EA4(work->billboard, 0);
    } else {
        sub_02023EA4(work->billboard, 1);
    }
    if (work->heightSet == 0) {
        VecFx32 curPos;
        VecFx32 tilePos;
        sub_02068DB8(manager, &curPos);
        tilePos.x = work->args.pos.x << 0x10;
        tilePos.z = work->args.pos.z << 0x10;
        tilePos.y = curPos.y;
        work->heightSet = sub_0206121C(work->args.fieldSystem, &tilePos);
        if (work->heightSet == 1) {
            curPos.y = tilePos.y;
            sub_02068DA8(manager, &curPos);
        }
    }
    switch (work->state) {
    case 0:
        sub_02023F04(work->billboard, 1 << 0xc);
        if (sub_02023F70(work->billboard) / 0x1000 >= 0xc) {
            work->state = 1;
        }
        break;
    case 1:
        sub_02023F1C(work->billboard, 3 << 0xe);
        sub_02023F04(work->billboard, 0);
        work->state = 2;

    case 2: {
        int x = MapObject_GetXCoord(obj) - work->args.unk_1C;
        int z = MapObject_GetZCoord(obj) - work->args.unk_1E;
        if (work->args.pos.x != x || work->args.pos.z != z) {
            ov01_021F1640(manager);
            return;
        }
        if (work->args.direction != -1 && work->args.direction != MapObject_GetFacingDirection(obj)) {
            ov01_021F1640(manager);
        }
        break;
    }
    }
}
