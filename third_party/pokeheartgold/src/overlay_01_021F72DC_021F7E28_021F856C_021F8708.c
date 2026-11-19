#include "global.h"
#include "billboard.h"

struct MapObject;

typedef struct MapObject MapObject;

fx32 sub_02023F30(const Billboard *billboard);

void MapObject_SetFacingVector(void *, VecFx32 *facingVector);

void ov01_021F8708(MapObject *mapObj, Billboard *param1, int param2);

void ov01_021F8708(MapObject *mapObj, Billboard *param1, int param2) {
    VecFx32 v0 = { 0, 0, 0 };
    int v1 = sub_02023F30(param1) / ((fx32)0x0000000000001000L);
    switch (param2) {
    case 0:
        break;

    case 1:
        if (v1 >= 24) {
            v0.z = ((fx32)0x0000000000001000L) * 6;
        }
        break;

    case 2:
        if (v1 < 40) {
            v0.x = ((fx32)0x0000000000001000L) * 1;
        } else {
            v0.x = ((fx32)0x0000000000001000L) * (-5);
        }
        break;

    case 3:
        if (v1 < 56) {
            v0.x = ((fx32)0x0000000000001000L) * -1;
        } else {
            v0.x = ((fx32)0x0000000000001000L) * 5;
        }
        break;
    }

    v0.z += 2 << 10;
    MapObject_SetFacingVector(mapObj, &v0);
}
