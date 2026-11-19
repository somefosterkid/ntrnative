#include "global.h"

#include "map_object.h"
#include "overlay_01_021F944C.h"

s32 ov01_021F8C88(LocalMapObject *, Billboard *);

s32 sub_02023EE0(Billboard *, s32);

s32 sub_02023F04(Billboard *, s32);

s32 sub_02023F40(Billboard *, s32);

void ov01_021F8AB0(LocalMapObject *arg0);

void ov01_021F8AB0(LocalMapObject *object) {
    u32 v0;
    Billboard *billboard;
    u32 v1;
    MapObjectBillboardWorkA *work;

    work = (MapObjectBillboardWorkA *)sub_0205F40C(object);
    billboard = work->billboard;
    if ((ov01_021FA2D4(object) != 1) && (billboard != NULL)) {
        v0 = MapObject_GetFacingDirection(object);
        v1 = sub_0205F330(object);
        if (v1 != work->unk_02) {
            if (v1 == 0) {
                sub_02023EE0(billboard, 0);
            } else {
                sub_02023EE0(billboard, 1);
            }
            sub_02023F40(billboard, 0);
        }
        sub_02023F04(billboard, 0x1000);
        MapObject_GetFacingVector(object)->z = 0x800;
        work->unk_00 = (u8)v0;
        work->unk_02 = (s8)v1;
        ov01_021FA3E8(object, billboard);
        ov01_021F8C88(object, billboard);
    }
}
