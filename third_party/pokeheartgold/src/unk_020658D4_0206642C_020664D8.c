#include "global.h"

#include "map_object.h"
#include "metatile_behavior.h"
#include "overlay_01.h"
#include "unk_02054648.h"

s32 ov01_021FF070(LocalMapObject *, s32);

s32 ov01_021FF8F0(LocalMapObject *, s32);

void sub_020664D8(LocalMapObject *arg0);

void sub_020664D8(LocalMapObject *object) {
    u32 x;
    u32 y;
    FieldSystem *fieldSystem;
    u32 z;
    u16 tile;
    u16 v0;

    fieldSystem = MapObject_GetFieldSystem(object);
    x = MapObject_GetXCoord(object);
    z = MapObject_GetZCoord(object);
    y = z;
    tile = GetMetatileBehavior(fieldSystem, (s32)x, (s32)z);
    if (MetatileBehavior_IsTallGrass(tile) == 1) {
        ov01_021FF070(object, 0);
    } else if (MetatileBehavior_IsVeryTallGrass(tile) == 1) {
        ov01_021FF8F0(object, 0);
    }
    if ((ov01_022055DC(object) != 0) && ((u32)(u8)((u8)MapObject_GetFacingDirection(object) + 0xFE) <= 1U)) {
        ov01_02205604(object, (s32 *)&x, (s32 *)&y);
        v0 = GetMetatileBehavior(fieldSystem, (s32)x, (s32)y);
        if (MetatileBehavior_IsTallGrass(v0) == 1) {
            ov01_021FF0E4(object, 1, (s32)x, (s32)y, 1);
            return;
        }
        if (MetatileBehavior_IsVeryTallGrass(v0) == 1) {
            ov01_021FF964(object, 1, (s32)x, (s32)y, 1);
        }
    }
}
