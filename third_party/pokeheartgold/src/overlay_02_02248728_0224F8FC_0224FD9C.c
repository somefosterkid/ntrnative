#include "global.h"

#include "field_types_def.h"
#include "follow_mon.h"
#include "map_object.h"

void ov01_021F8F74(LocalMapObject *, s32);

void ov02_0224FD9C(u8 *param_1, LocalMapObject *param_2);

void ov02_0224FD9C(u8 *param_1, LocalMapObject *param_2) {
    s32 iVar1;
    VecFx32 VStack_18;

    MapObject_CopyPositionVector(param_2, &VStack_18);
    if (*(s8 *)(param_1 + 2) != 0) {
        VStack_18.x = VStack_18.x + *(s8 *)(param_1 + 2) * 0x1000;
    }
    if (((*(s8 *)(param_1 + 3) != 0) && (iVar1 = FollowMon_GetSpecies(param_2), iVar1 != 0x32))
        && (iVar1 != 0x33)) {
        ov01_021F8F74(param_2, (s32) * (s8 *)(param_1 + 3));
    }
    if (*(s8 *)(param_1 + 4) != 0) {
        VStack_18.z = VStack_18.z + *(s8 *)(param_1 + 4) * 0x1000;
    }
    MapObject_SetPositionVector(param_2, &VStack_18);
    return;
}
