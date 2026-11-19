#include "global.h"
#include "map_object.h"

struct LocalMapObject;

struct ObjectEventGfxRenderDetailsEntry;

typedef struct LocalMapObject MapObject;

int sub_02061108(MapObject *mapObj);




const ObjectEventGfxRenderDetailsEntry *ov5_021ECD04(const MapObject *param0);

void MapObject_SinkIntoTerrain(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);

void sub_0206039C(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);

void sub_02063C94(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);

void sub_02063CFC(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);

void sub_02063DA8(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);

void sub_0206008C(MapObject *mapObj);

const ObjectEventGfxRenderDetailsEntry *ov01_021F9318(const MapObject *param0);

void sub_020601A4(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);

void sub_0206073C(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);

void sub_020607A4(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);

void sub_020609D4(MapObject *mapObj, s32 currTileBehavior, s32 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);

void sub_02060530(MapObject *mapObj, u8 currTileBehavior, s32 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);


void sub_0206008C(MapObject *mapObj) {
    u8 v1 = sub_0205F514(mapObj);
    sub_0205F51C(mapObj, v1);
    sub_02061108(mapObj);

    if (sub_0205F73C(mapObj) == TRUE) {
        u8 currTileBehavior = sub_0205F504(mapObj);
        u8 prevTileBehavior = sub_0205F514(mapObj);
        const ObjectEventGfxRenderDetailsEntry *v2 = ov01_021F9318(mapObj);

        sub_020601A4(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_0206073C(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_020607A4(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_0206039C(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_020609D4(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_02060530(mapObj, currTileBehavior, prevTileBehavior, v2);
    }
}
