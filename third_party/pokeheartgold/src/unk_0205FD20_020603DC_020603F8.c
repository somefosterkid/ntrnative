#include "global.h"

#include "map_object.h"
#include "metatile_behavior.h"
#include "unk_0205FD20.h"

s32 ov01_021FD640(LocalMapObject *);

s32 ov01_021FD684(LocalMapObject *);

void sub_020603F8(LocalMapObject *arg0, u8 arg1, u8 arg2, u16 *arg3);

void sub_020603F8(LocalMapObject *object, u8 tile, u8 arg2, u16 *arg3) {
    u32 v0;

    if ((MapObjectManager_NotEndMovement(MapObject_GetManager(object)) == 0) || (v0 = (u32)(*arg3 << 0x19) >> 0x1D, (v0 == 0))) {
        return;
    }
    if (v0 == 2) {
        if ((MetatileBehavior_IsTallGrass(tile) == 1) || (MetatileBehavior_IsVeryTallGrass(tile) == 1) || (MetatileBehavior_IsTallGrass(arg2) == 1) || (MetatileBehavior_IsVeryTallGrass(arg2) == 1) || (sub_02060E54(object, (s32)tile) == 1) || (MetatileBehavior_IsPuddle(tile) == 1) || (MetatileBehavior_IsShallowWater(tile) == 1) || (sub_02060EBC(object, (s32)tile) == 1) || (MetatileBehavior_IsMud(tile) == 1) || (MetatileBehavior_IsReflective(tile) != 0)) {
            MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK20);
            return;
        }
        if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_UNK15) == 0) {
            ov01_021FD684(object);
            MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK15);
        }
    } else {
        if ((MetatileBehavior_IsTallGrass(tile) == 1) || (MetatileBehavior_IsVeryTallGrass(tile) == 1) || (sub_02060E54(object, (s32)tile) == 1) || (MetatileBehavior_IsPuddle(tile) == 1) || (MetatileBehavior_IsShallowWater(tile) == 1) || (sub_02060EBC(object, (s32)tile) == 1) || (MetatileBehavior_IsMud(tile) == 1) || (MetatileBehavior_IsReflective(tile) != 0)) {
            MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK20);
            return;
        }
        if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_UNK15) == 0) {
            ov01_021FD640(object);
            MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK15);
        }
    }
}
