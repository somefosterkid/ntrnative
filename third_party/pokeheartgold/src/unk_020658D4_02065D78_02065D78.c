#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "map_object.h"
#include "player_avatar.h"

u32 sub_02065D78(LocalMapObject *arg0);

u32 sub_02065D78(LocalMapObject *arg0) {
    FieldSystem *temp_r0;
    u32 temp_r1;
    u32 var_r0;

    temp_r0 = MapObject_GetFieldSystem(arg0);
    FieldSystem_GetPlayerAvatar(temp_r0);
    var_r0 = temp_r0->followMon.unk4;
    temp_r1 = var_r0 - 0x58;
    switch (temp_r1) {
    case 0:
        return 0x10U;
    case 1:
        return 0x11U;
    case 2:
        return 0x12U;
    case 3:
        var_r0 = 0x13;
    default:
        return var_r0;
    }
}
