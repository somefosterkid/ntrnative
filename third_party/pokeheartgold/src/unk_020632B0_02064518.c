#include "global.h"

#include "map_object.h"

int sub_02064518(LocalMapObject *mapObj);

int sub_02064518(LocalMapObject *mapObj) {
    return MapObject_GetTrainerNum(mapObj);
}
