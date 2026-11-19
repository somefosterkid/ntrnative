#include "global.h"

#include "map_object.h"
#include "overlay_01_021F944C.h"

s32 ov01_021F8C88(LocalMapObject *, Billboard *);

s32 sub_02023EE0(Billboard *, s32);

s32 sub_02023EF4(Billboard *);

s32 sub_02023F04(Billboard *, s32);

s32 sub_02023F40(Billboard *, s32);

void ov01_021F894C(LocalMapObject *arg0);

void ov01_021F894C(LocalMapObject *object) {
    Billboard *billboard;
    MapObjectBillboardWorkA *work;

    work = (MapObjectBillboardWorkA *)sub_0205F40C(object);
    billboard = work->billboard;
    if ((ov01_021FA2D4(object) != 1) && (billboard != NULL)) {
        work->unk_00 = (u8)MapObject_GetFacingDirection(object);
        work->unk_02 = (s8)sub_0205F330(object);
        if (sub_02023EF4(billboard) != 0) {
            sub_02023EE0(billboard, 0);
            sub_02023F40(billboard, 0);
        }
        sub_02023F04(billboard, 0x1000);
        MapObject_GetFacingVector(object)->z = 0x800;
        ov01_021FA3E8(object, billboard);
        ov01_021F8C88(object, billboard);
    }
}

void ov01_021F89B8(LocalMapObject *arg0);

void ov01_021F89B8(LocalMapObject *object) {
    Billboard *billboard;
    u32 v1;
    MapObjectBillboardWorkA *work;

    work = (MapObjectBillboardWorkA *)sub_0205F40C(object);
    billboard = work->billboard;
    if ((ov01_021FA2D4(object) != 1) && (billboard != NULL)) {
        v1 = MapObject_GetFacingDirection(object);
        if (sub_0205F330(object) == 0) {
            if (sub_02023EF4(billboard) != 1) {
                sub_02023EE0(billboard, 1);
                sub_02023F40(billboard, 0);
            }
            sub_02023F04(billboard, 0x1000);
        } else {
            if (sub_02023EF4(billboard) != 0) {
                sub_02023EE0(billboard, 0);
                sub_02023F40(billboard, 0);
            }
            sub_02023F04(billboard, 0x1000);
        }
        work->unk_00 = (u8)v1;
        work->unk_02 = (s8)sub_0205F330(object);
        ov01_021FA3E8(object, billboard);
        ov01_021F8C88(object, billboard);
    }
}
