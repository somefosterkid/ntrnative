#include "global.h"

#include "error_handling.h"
#include "field_types_def.h"
#include "map_object.h"

void ov02_0224FF04(LocalMapObject *param_1, u32 param_2, u32 *param_3, u32 *param_4);

void ov02_0224FF04(LocalMapObject *param_1, u32 param_2, u32 *param_3, u32 *param_4) {
    u32 uVar1;

    uVar1 = MapObject_GetXCoord(param_1);
    *param_3 = uVar1;
    uVar1 = MapObject_GetZCoord(param_1);
    *param_4 = uVar1;
    switch (param_2) {
    case 0:
        *param_4 = *param_4 + 1;
        return;
    case 1:
        *param_4 = *param_4 - 1;
        return;
    case 2:
        *param_3 = *param_3 + 1;
        return;
    case 3:
        *param_3 = *param_3 - 1;
        return;
    default:
        GF_AssertFail();
        return;
    }
}
