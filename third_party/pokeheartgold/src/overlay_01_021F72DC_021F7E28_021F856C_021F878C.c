#include "global.h"

#include "map_object.h"
#include "overlay_01_021F944C.h"
#include "billboard.h"

void ov01_021F8C88(LocalMapObject *mapObj, Billboard *param1);

void ov01_021F8874(LocalMapObject *mapObj, Billboard *param1, int param2);

extern const MapObjectBillboardStateFuncE ov01_022089D0[2];

void ov01_021F856C(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3);

void ov01_021F85BC(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3);

void ov01_021F85F4(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3);

void ov01_021F86B8(LocalMapObject *mapObj, Billboard *param1, MapObjectBillboardWorkE *param2, int param3);

void ov01_021F878C(LocalMapObject *mapObj);

void ov01_021F878C(LocalMapObject *mapObj) {
    int v0;
    int v1;
    MapObjectBillboardWorkE *v2 = (MapObjectBillboardWorkE *)sub_0205F40C(mapObj);
    Billboard *v3 = v2->billboard;
    if ((v3 == 0L) || (ov01_021FA2D4(mapObj) == 1)) {
        return;
        v2 = (MapObjectBillboardWorkE *)sub_0205F40C(mapObj);
    }
    v0 = MapObject_GetFacingDirection(mapObj);
    v1 = sub_0205F330(mapObj);
    (v1 < (0x1 + 1)) ? ((void)0) : (GF_AssertFail());
    ov01_022089D0[v1](mapObj, v3, v2, v0);
    ov01_021F8874(mapObj, v3, v0);
    v2->unk_00 = v0;
    v2->unk_02 = sub_0205F330(mapObj);
    ov01_021FA3E8(mapObj, v3);
    ov01_021F8C88(mapObj, v3);
}
