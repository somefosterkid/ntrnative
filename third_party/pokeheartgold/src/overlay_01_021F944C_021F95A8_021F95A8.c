#include "global.h"

#include "map_object.h"
#include "overlay_01_021F944C.h"

s32 ov01_021F9688(void *arg0);

void ov01_021F95A8(LocalMapObject *object, Billboard **billboard) {
    ov01_021F9688(object);
    if (*billboard != NULL) {
        ov01_021F9574(object, billboard, MapObject_GetSpriteID(object));
    }
}
