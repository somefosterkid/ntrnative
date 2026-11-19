#include "global.h"

#include "map_object.h"
#include "metatile_behavior.h"
#include "unk_0205FD20.h"

void sub_02060530(LocalMapObject *arg0, u8 arg1, s32 arg2, u16 *arg3);

void sub_02060530(LocalMapObject *arg0, u8 arg1, s32 arg2, u16 *arg3) {
    u16 temp_r6;
    u32 temp_r0;

    if (MapObjectManager_NotEndMovement(MapObject_GetManager(arg0)) == 0) {
        return;
    }
    temp_r0 = (u32)(*arg3 << 0x19) >> 0x1D;
    if (temp_r0 != 0) {
        if (temp_r0 == 2) {
            temp_r6 = (u32)sub_0205F524(arg0);
            if ((MetatileBehavior_IsTallGrass(arg1) == 1) || (MetatileBehavior_IsVeryTallGrass(arg1) == 1) || (MetatileBehavior_IsTallGrass((u8)temp_r6) == 1) || (MetatileBehavior_IsVeryTallGrass((u8)temp_r6) == 1) || (sub_02060E54(arg0, (s32)arg1) == 1) || (MetatileBehavior_IsPuddle(arg1) == 1) || (MetatileBehavior_IsShallowWater(arg1) == 1) || (sub_02060EBC(arg0, (s32)arg1) == 1) || (MetatileBehavior_IsMud(arg1) == 1) || (MetatileBehavior_IsReflective(arg1) != 0)) {
                MapObject_SetFlagsBits(arg0, MAPOBJECTFLAG_UNK20);
                return;
            }
            MapObject_ClearFlagsBits(arg0, MAPOBJECTFLAG_UNK20);
            return;
        }
        if ((MetatileBehavior_IsTallGrass(arg1) == 1) || (MetatileBehavior_IsVeryTallGrass(arg1) == 1) || (sub_02060E54(arg0, (s32)arg1) == 1) || (MetatileBehavior_IsPuddle(arg1) == 1) || (MetatileBehavior_IsShallowWater(arg1) == 1) || (sub_02060EBC(arg0, (s32)arg1) == 1) || (MetatileBehavior_IsMud(arg1) == 1) || (MetatileBehavior_IsReflective(arg1) != 0)) {
            MapObject_SetFlagsBits(arg0, MAPOBJECTFLAG_UNK20);
            return;
        }
        MapObject_ClearFlagsBits(arg0, MAPOBJECTFLAG_UNK20);
    }
}
