#include "global.h"

#include "error_handling.h"
#include "map_object.h"

extern u32 ov01_022074A8;

const ObjectEventGfxRenderDetailsEntry *ov01_021F9318(LocalMapObject *object);

ObjectEventGraphicsInfo *ObjectEvent_GetGraphicsInfo(u32 param_1) {
    ObjectEventGraphicsInfo *puVar1;

    puVar1 = (ObjectEventGraphicsInfo *)&ov01_022074A8;
    do {
        if (puVar1->spriteId == param_1) {
            return puVar1;
        }
        puVar1 = puVar1 + 1;
    } while (puVar1->spriteId != 0xffff);
    GF_AssertFail();
    return NULL;
}

u32 GetMoveModelNoBySpriteId(u32 param_1);

u32 GetMoveModelNoBySpriteId(u32 param_1) {
    ObjectEventGraphicsInfo *puVar1;

    puVar1 = ObjectEvent_GetGraphicsInfo(param_1);
    if (puVar1 == NULL) {
        return 0xffffffff;
    }
    return (u32)puVar1->mapModelId;
}

const ObjectEventGfxRenderDetailsEntry *ov01_021F9318(LocalMapObject *object) {
    return ov01_021F9324(MapObject_GetSpriteID(object));
}
