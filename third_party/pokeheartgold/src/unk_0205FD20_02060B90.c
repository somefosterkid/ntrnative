#include "global.h"

struct MapObject;
typedef struct MapObject MapObject;
u32 sub_02060AF4(const MapObject *mapObj, const VecFx32 *pos, int x, int y, int z, int dir);
void MapObject_CopyPositionVector(const MapObject *mapObj, VecFx32 *pos);

u32 sub_02060B90(const MapObject *mapObj, int x, int y, int z, int dir);

u32 sub_02060B90(const MapObject *mapObj, int x, int y, int z, int dir) {
    VecFx32 pos;

    MapObject_CopyPositionVector(mapObj, &pos);
    return sub_02060AF4(mapObj, &pos, x, y, z, dir);
}
