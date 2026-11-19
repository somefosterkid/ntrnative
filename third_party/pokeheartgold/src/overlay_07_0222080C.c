#include "global.h"

struct MapObject;
typedef struct MapObject MapObject;
void ov07_022207D0(MapObject *mapObj, int param1, u32 param2);

void ov07_0222080C(MapObject *mapObj);

void ov07_0222080C(MapObject *mapObj) {
    ov07_022207D0(mapObj, 1, 1);
}
