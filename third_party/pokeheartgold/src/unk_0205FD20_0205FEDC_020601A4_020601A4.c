#include "global.h"
#include "map_object.h"

struct LocalMapObject;

struct ObjectEventGfxRenderDetailsEntry;

typedef struct LocalMapObject MapObject;


void sub_020601A4(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);

void sub_020601A4(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails) {
    VecFx32 spritePosOffset = { 0, 0, 0 };

    sub_0205F9C0(mapObj, &spritePosOffset);
}
