#include "global.h"
#include "map_object.h"

struct MapObject;

struct ObjectEventGfxRenderDetailsEntry;

typedef struct MapObject MapObject;

void ov14_021F40E8(MapObject *mapObj, int param1);

void ov14_021F40DC(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);

void ov14_021F40DC(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails) {
    ov14_021F40E8(mapObj, 0);
}
