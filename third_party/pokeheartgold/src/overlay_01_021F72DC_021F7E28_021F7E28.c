#include "global.h"

#include "map_object.h"
#include "overlay_01_021F944C.h"

void ov01_021F7E28(LocalMapObject *arg0);

void ov01_021F7E28(LocalMapObject *arg0) {
    MapObjectBillboardWorkA *work;

    work = (MapObjectBillboardWorkA *)sub_0205F40C(arg0);
    if (ov01_021FA2D4(arg0) != 1) {
        if (work->billboard == NULL) {
            ov01_021F9510(arg0, &work->billboard);
        }
        if (work->billboard != NULL) {
            ov01_021F9630(work->billboard, &work->animState);
            ov01_021FA3E8(arg0, work->billboard);
            MapObject_ClearFlagsBits(arg0, MAPOBJECTFLAG_UNK21);
        }
    }
}
