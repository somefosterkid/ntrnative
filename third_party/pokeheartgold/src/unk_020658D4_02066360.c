#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"

OverworldAnimManager *sub_0206642C(LocalMapObject *mapObj);
void sub_02066360(LocalMapObject *mapObj);

void sub_02066360(LocalMapObject *mapObj) {
    OverworldAnimManager *a0;

    a0 = sub_0206642C(mapObj);
    if (a0 != 0) {
        ov01_021F1640(a0);
    }
    return;
}
