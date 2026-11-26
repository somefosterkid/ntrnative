#include "global.h"

#include "map_object.h"
#include "overlay_01_021F944C.h"

void ov01_021F8C88(LocalMapObject *, Billboard *);

void ov01_021F77A4(LocalMapObject *arg0);

void ov01_021F77A4(LocalMapObject *obj) {
    Billboard *billboard;
    MapObjectBillboardWorkA *work;

    work = (MapObjectBillboardWorkA *)sub_0205F40C(obj);
    if (ov01_021FA2D4(obj) != 1) {
        billboard = work->billboard;
        if (billboard != NULL) {
            ov01_021FA3E8(obj, billboard);
            ov01_021F8C88(obj, billboard);
        }
    }
}

s32 ov01_021F9344(LocalMapObject *);

s32 sub_02023F04(Billboard *, s32);

void ov01_021F77D0(LocalMapObject *arg0);

void ov01_021F77D0(LocalMapObject *obj) {
    Billboard *billboard;
    MapObjectBillboardWorkA *work;

    work = (MapObjectBillboardWorkA *)sub_0205F40C(obj);
    if (ov01_021FA2D4(obj) != 1) {
        billboard = work->billboard;
        if (billboard != NULL) {
            if (ov01_021F9344(obj) == 0) {
                sub_02023F04(billboard, 0x1000);
            }
            ov01_021FA3E8(obj, billboard);
            ov01_021F8C88(obj, billboard);
        }
    }
}
