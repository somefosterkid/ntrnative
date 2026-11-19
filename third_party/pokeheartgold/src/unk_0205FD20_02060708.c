#include "global.h"

#include "metatile_behavior.h"
#include "map_object.h"

struct LocalMapObject;
struct ObjectEventGfxRenderDetailsEntry;
typedef struct LocalMapObject MapObject;
void ov01_021FECA0(MapObject *param0, int param1, int param2, int param3);
void ov01_021FEE04(MapObject *param0, int param1, int param2, int param3);

void sub_02060708(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);
void sub_0206073C(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);
void sub_02060770(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);
void sub_020607A4(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);

void sub_02060708(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails) {
    if (MetatileBehavior_IsPuddle(prevTileBehavior) == 1) {
        ov01_021FECA0(mapObj, MapObject_GetPreviousXCoord(mapObj), MapObject_GetPreviousYCoord(mapObj), MapObject_GetPreviousZCoord(mapObj));
    }
}

void sub_0206073C(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails) {
    if (MetatileBehavior_IsPuddle(currTileBehavior) == 1) {
        ov01_021FECA0(mapObj, MapObject_GetXCoord(mapObj), MapObject_GetYCoord(mapObj), MapObject_GetZCoord(mapObj));
    }
}

void sub_02060770(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails) {
    if (MetatileBehavior_IsMud(prevTileBehavior) == 1) {
        ov01_021FEE04(mapObj, MapObject_GetPreviousXCoord(mapObj), MapObject_GetPreviousYCoord(mapObj), MapObject_GetPreviousZCoord(mapObj));
    }
}

void sub_020607A4(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails) {
    if (MetatileBehavior_IsMud(currTileBehavior) == 1) {
        ov01_021FEE04(mapObj, MapObject_GetXCoord(mapObj), MapObject_GetYCoord(mapObj), MapObject_GetZCoord(mapObj));
    }
}
