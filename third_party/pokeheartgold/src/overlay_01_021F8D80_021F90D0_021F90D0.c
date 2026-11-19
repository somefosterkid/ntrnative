#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "overlay_01_021F944C.h"
#include "sprite.h"

void ov01_021F90D0(LocalMapObject *param_1);

void ov01_021F90D0(LocalMapObject *param_1) {
    MapObjectBillboardWorkC *work;

    work = (MapObjectBillboardWorkC *)sub_0205F40C(param_1);
    if (work->billboard != NULL) {
        ov01_021F9610(work->billboard, &work->animState);
    }
    ov01_021F95A8(param_1, &work->billboard);
    MapObject_SetFlagsBits(param_1, MAPOBJECTFLAG_UNK21);
    return;
}

void ov01_021F90FC(LocalMapObject *param_1);

void ov01_021F90FC(LocalMapObject *param_1) {
    MapObjectBillboardWorkC *work;
    BOOL BVar2;

    work = (MapObjectBillboardWorkC *)sub_0205F40C(param_1);
    BVar2 = ov01_021FA2D4(param_1);
    if (BVar2 != 1) {
        if (work->billboard == NULL) {
            ov01_021F9510(param_1, &work->billboard);
        }
        if (work->billboard != NULL) {
            ov01_021F9630(work->billboard, &work->animState);
            ov01_021FA3E8(param_1, work->billboard);
            MapObject_ClearFlagsBits(param_1, MAPOBJECTFLAG_UNK21);
        }
    }
    return;
}
