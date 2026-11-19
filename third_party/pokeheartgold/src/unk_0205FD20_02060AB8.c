#include "global.h"

#include "metatile_behavior.h"
#include "map_object.h"

struct LocalMapObject;
struct ObjectEventGfxRenderDetailsEntry;
typedef struct LocalMapObject MapObject;

void sub_02060AB8(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);

void sub_02060AF0(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);

void sub_02060AB8(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails) {
    if (sub_0205BA24(currTileBehavior) == 1) {
        MapObject_SetFlag28(mapObj, 1);
    } else if (MapObject_CheckFlag28(mapObj) == 1 && sub_0205BA30(currTileBehavior) == 0) {
        MapObject_SetFlag28(mapObj, 0);
    }
}

void sub_02060AF0(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails) {
}
