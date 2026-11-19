#include "global.h"

#include "error_handling.h"
#include "map_object.h"
#include "overlay_01_021F944C.h"

void sub_0206402C(LocalMapObject *arg0);

void sub_0206402C(LocalMapObject *arg0) {
    s32 temp_r2;
    MapObjectBillboardWorkD *work;

    work = (MapObjectBillboardWorkD *)sub_0205F40C(arg0);
    if (ov01_021FA2D4(arg0) != 1) {
        if (work->billboard == NULL) {
            temp_r2 = work->spriteId;
            if (temp_r2 != 0xFFFF) {
                ov01_021F94C0(arg0, &work->billboard, temp_r2);
            } else {
                GF_AssertFail();
            }
        }
        if (work->billboard != NULL) {
            ov01_021F9630(work->billboard, &work->animState);
            ov01_021FA3E8(arg0, work->billboard);
            MapObject_ClearFlagsBits(arg0, MAPOBJECTFLAG_UNK21);
        }
    }
}
