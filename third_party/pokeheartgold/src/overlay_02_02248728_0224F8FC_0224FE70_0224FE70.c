#include "global.h"

#include "field_system.h"
#include "map_object.h"
#include "metatile_behavior.h"
#include "overlay_01.h"
#include "unk_02054648.h"

s32 ov02_0224FF04(LocalMapObject *, u32, s32 *, s32 *);

void ov02_0224FE70(FieldSystemUnkSub120 *arg0, LocalMapObject *arg1, s32 arg2);

void ov02_0224FE70(FieldSystemUnkSub120 *arg0, LocalMapObject *object, s32 arg2) {
    s32 x;
    s32 y;
    FieldSystem *fieldSystem;
    u8 v0;
    u16 tile;

    if (ov01_022055DC(object) == 0 || arg2 == (u8)MapObject_GetFacingDirection(object) || !(v0 = arg0->unk87C, (v0 == 2)) && v0 != 3) {
        return;
    }
    fieldSystem = MapObject_GetFieldSystem(object);
    ov02_0224FF04(object, arg0->unk87C, &x, &y);
    tile = GetMetatileBehavior(fieldSystem, x, y);
    if (MetatileBehavior_IsTallGrass(tile) == 1) {
        ov01_021FF0E4(object, 0, x, y, 1);
        return;
    }
    if (MetatileBehavior_IsVeryTallGrass(tile) == 1) {
        ov01_021FF964(object, 0, x, y, 1);
    }
}
