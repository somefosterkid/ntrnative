#include "global.h"

struct MapObject;
typedef struct MapObject MapObject;
int sub_02061070(MapObject *mapObj);
u32 MapObject_GetFlagsBitsMask(const MapObject *mapObj, u32 flag);

void sub_0205FE0C(MapObject *mapObj);

int sub_02061108(MapObject *mapObj);
void MapObject_SetStartMovement(MapObject *mapObj);
void MapObject_ClearFlagsBits(MapObject *mapObj, u32 flag);
void sub_0205FEDC(MapObject *mapObj);
void sub_0205FF6C(MapObject *mapObj);
void sub_02060020(MapObject *mapObj);
void sub_0206008C(MapObject *mapObj);
void sub_02060114(MapObject *mapObj);
void sub_0205FE24(MapObject *mapObj);
void sub_0205FE48(MapObject *mapObj);
void sub_0205FE6C(MapObject *mapObj);
void sub_0205FEA4(MapObject *mapObj);

struct ObjectEventGfxRenderDetailsEntry;
u16 sub_0205F504(const MapObject *mapObj);
u16 sub_0205F514(const MapObject *mapObj);
int sub_0205F73C(const MapObject *mapObj);
typedef struct ObjectEventGfxRenderDetailsEntry ObjectEventGfxRenderDetailsEntry;
const ObjectEventGfxRenderDetailsEntry *ov01_021F9318(const MapObject *param0);
void sub_020601A4(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);
void sub_020601BC(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);
void sub_0206039C(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);
void sub_020603DC(MapObject *mapObj, s32 currTileBehavior, s32 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);
void sub_02060698(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);
void sub_02060700(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);
void sub_020607D8(MapObject *mapObj, u8 currTileBehavior, u32 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);
void sub_02060AB8(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);
void sub_02060274(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);
void sub_02060328(MapObject *mapObj, s32 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);
void sub_020603F8(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);
void sub_020606CC(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);
void sub_02060704(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);
void sub_02060708(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);
void sub_02060770(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);
void sub_02060AF0(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);
void sub_020603D0(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);

void sub_0205FE0C(MapObject *mapObj) {
    if (MapObject_GetFlagsBitsMask(mapObj, (1 << 12))) {
        sub_02061070(mapObj);
    }
}

void sub_0205FE24(MapObject *mapObj) {
    if (MapObject_GetFlagsBitsMask(mapObj, (1 << 11)) && sub_02061108(mapObj) == 1) {
        MapObject_SetStartMovement(mapObj);
    }
}

void sub_0205FE48(MapObject *mapObj) {
    if (MapObject_GetFlagsBitsMask(mapObj, (1 << 2))) {
        sub_0205FEDC(mapObj);
    }

    MapObject_ClearFlagsBits(mapObj, (1 << 2) | (1 << 16));
}

void sub_0205FE6C(MapObject *mapObj) {
    if (MapObject_GetFlagsBitsMask(mapObj, (1 << 16))) {
        sub_02060020(mapObj);
    } else if (MapObject_GetFlagsBitsMask(mapObj, (1 << 2))) {
        sub_0205FF6C(mapObj);
    }

    MapObject_ClearFlagsBits(mapObj, (1 << 2) | (1 << 16));
}

void sub_0205FEA4(MapObject *mapObj) {
    if (MapObject_GetFlagsBitsMask(mapObj, (1 << 17))) {
        sub_02060114(mapObj);
    } else if (MapObject_GetFlagsBitsMask(mapObj, (1 << 3))) {
        sub_0206008C(mapObj);
    }

    MapObject_ClearFlagsBits(mapObj, (1 << 3) | (1 << 17));
}

void sub_0205FEDC(MapObject *mapObj) {
    sub_02061108(mapObj);

    if (sub_0205F73C(mapObj) == 1) {
        u8 currTileBehavior = sub_0205F504(mapObj);
        u8 prevTileBehavior = sub_0205F514(mapObj);
        const ObjectEventGfxRenderDetailsEntry *v2 = ov01_021F9318(mapObj);

        sub_02060AB8(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_020601BC(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_0206039C(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_020603DC(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_020601A4(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_02060698(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_02060700(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_020607D8(mapObj, currTileBehavior, prevTileBehavior, v2);
    }
}

void sub_0205FF6C(MapObject *mapObj) {
    sub_02061108(mapObj);

    if (sub_0205F73C(mapObj) == 1) {
        u8 currTileBehavior = sub_0205F504(mapObj);
        u8 prevTileBehavior = sub_0205F514(mapObj);
        const ObjectEventGfxRenderDetailsEntry *v2 = ov01_021F9318(mapObj);

        sub_02060AB8(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_02060274(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_02060328(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_0206039C(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_020603F8(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_020606CC(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_02060704(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_02060708(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_02060770(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_020607D8(mapObj, currTileBehavior, prevTileBehavior, v2);

        sub_02060AF0(mapObj, currTileBehavior, prevTileBehavior, v2);
    }
}

void sub_02060020(MapObject *mapObj) {
    sub_02061108(mapObj);

    if (sub_0205F73C(mapObj) == 1) {
        u8 currTileBehavior = sub_0205F504(mapObj);
        u8 prevTileBehavior = sub_0205F514(mapObj);
        const ObjectEventGfxRenderDetailsEntry *v2 = ov01_021F9318(mapObj);

        sub_02060AB8(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_020603F8(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_020607D8(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_020603D0(mapObj, currTileBehavior, prevTileBehavior, v2);
        sub_02060AF0(mapObj, currTileBehavior, prevTileBehavior, v2);
    }
}
