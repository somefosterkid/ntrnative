#include "global.h"

#include "metatile_behavior.h"
#include "overlay_01_021FE200.h"
#include "map_object.h"

s32 ov01_021FE3F4(void *);

s32 sub_02060EA4(void *, u16);

u32 sub_02060ED4(void *, u16);

void sub_02060328(void *arg0, s32 arg1, u8 arg2, u16 *arg3);

struct LocalMapObject;
struct ObjectEventGfxRenderDetailsEntry;
typedef struct LocalMapObject MapObject;
void ov01_021FF4FC(MapObject *param0, int param1);
void sub_0206039C(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);
void sub_020603D0(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);

void sub_02060328(void *arg0, s32 arg1, u8 tile, u16 *arg3) {
    u32 v0;
    u32 v1;

    v1 = (u32)(*arg3 << 0x15) >> 0x1C;
    if (v1 != 0) {
        if (sub_0205BA94(tile) == 1) {
            v0 = (u32)(*arg3 << 0x15) >> 0x1C;
            if (v0 == 1) {
                ov01_021FE3E8(arg0);
            } else if (v0 == 2) {
                ov01_021FE3F4(arg0);
            }
        }
        if (sub_02060EA4(arg0, tile) == 1) {
            v1 = (u32)(*arg3 << 0x15) >> 0x1C;
            if (v1 == 1) {
                ov01_021FE3C4(arg0);
            } else if (v1 == 2) {
                ov01_021FE3D0(arg0);
            }
            return;
        }
        v1 = sub_02060ED4(arg0, tile);
        if (v1 == 1) {
            ov01_021FE3DC(arg0);
        }
    }
}

void sub_0206039C(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails) {
    if (MetatileBehavior_IsShallowWater(currTileBehavior) == 1) {
        if (MapObject_CheckFlag26(mapObj) == 0) {
            ov01_021FF4FC(mapObj, 1);
            MapObject_SetFlag26(mapObj, 1);
        }
    } else {
        MapObject_SetFlag26(mapObj, 0);
    }
}

void sub_020603D0(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails) {
    MapObject_SetFlag26(mapObj, 0);
}
