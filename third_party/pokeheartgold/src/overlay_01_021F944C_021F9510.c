#include "global.h"

#include "map_object.h"
#include "overlay_01_021F944C.h"

int ov01_021F9510(LocalMapObject *obj, Billboard **billboard) {
    return ov01_021F94C0(obj, billboard, MapObject_GetSpriteID(obj));
}
