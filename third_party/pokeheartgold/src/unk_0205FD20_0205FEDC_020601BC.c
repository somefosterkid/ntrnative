#include "global.h"

#include "map_object.h"
#include "metatile_behavior.h"
#include "overlay_01.h"
#include "unk_02054648.h"

s32 ov01_021FF070(LocalMapObject *, s32);

void sub_020601BC(LocalMapObject *object, u8 tile, u8 prevTile, const ObjectEventGfxRenderDetailsEntry *renderDetails);

void sub_020601BC(LocalMapObject *object, u8 tile, u8 prevTile, const ObjectEventGfxRenderDetailsEntry *renderDetails) {
    s32 x;
    s32 y;
    FieldSystem *fieldSystem;
    u32 v1;
    u16 v0;
    u8 v2;

    v1 = MapObject_GetID(object);
    if (MetatileBehavior_IsTallGrass(tile) == 1) {
        if ((v1 != 0xFD) || (MapObject_TestFlagsBits(object, MAPOBJECTFLAG_VISIBLE) != 1)) {
            ov01_021FF070(object, 0);
            goto block_4;
        }
    } else {
block_4:
        if ((MapObject_GetID(object) == 0xFD) && (ov01_022055DC(object) != 0) && (v2 = (u8)MapObject_GetFacingDirection(object), fieldSystem = MapObject_GetFieldSystem(object), ((u32)(u8)(v2 + 0xFE) <= 1U))) {
            ov01_02205604(object, &x, &y);
            v0 = GetMetatileBehavior(fieldSystem, x, y);
            if (MetatileBehavior_IsTallGrass(v0) == 1) {
                ov01_021FF0E4(object, 0, x, y, 1);
                return;
            }
            if (MetatileBehavior_IsVeryTallGrass(v0) == 1) {
                ov01_021FF964(object, 0, x, y, 1);
            }
        }
    }
}

void sub_02060274(LocalMapObject *object, u8 tile, u8 prevTile, const ObjectEventGfxRenderDetailsEntry *renderDetails);

void sub_02060274(LocalMapObject *object, u8 tile, u8 prevTile, const ObjectEventGfxRenderDetailsEntry *renderDetails) {
    s32 x;
    s32 y;
    FieldSystem *fieldSystem;
    u32 v1;
    u16 v0;
    u8 v2;

    v1 = MapObject_GetID(object);
    if (MetatileBehavior_IsTallGrass(tile) == 1) {
        if ((v1 != 0xFD) || (MapObject_TestFlagsBits(object, MAPOBJECTFLAG_VISIBLE) != 1)) {
            ov01_021FF070(object, 1);
            goto block_4;
        }
    } else {
block_4:
        if ((MapObject_GetID(object) == 0xFD) && (ov01_022055DC(object) != 0) && (v2 = (u8)MapObject_GetFacingDirection(object), fieldSystem = MapObject_GetFieldSystem(object), ((u32)(u8)(v2 + 0xFE) <= 1U))) {
            ov01_02205604(object, &x, &y);
            v0 = GetMetatileBehavior(fieldSystem, x, y);
            if (MetatileBehavior_IsTallGrass(v0) == 1) {
                ov01_021FF0E4(object, 1, x, y, 1);
                return;
            }
            if (MetatileBehavior_IsVeryTallGrass(v0) == 1) {
                ov01_021FF964(object, 1, x, y, 1);
            }
        }
    }
}
