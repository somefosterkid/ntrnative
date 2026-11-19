#include "global.h"

#include "field_types_def.h"
#include "map_object.h"

void sub_020661CC(void *param_1, u8 *param_2, LocalMapObject *param_3);

void sub_020661CC(void *param_1, u8 *param_2, LocalMapObject *param_3) {
    u32 uVar1;

    *(u8 *)(param_2 + 1) = 1;
    uVar1 = MapObject_GetXCoord(param_3);
    *(s16 *)(param_2 + 2) = (s16)uVar1;
    uVar1 = MapObject_GetZCoord(param_3);
    *(s16 *)(param_2 + 4) = (s16)uVar1;
    *(u16 *)(param_2 + 6) = 0xff;
    *(LocalMapObject **)(param_2 + 8) = param_3;
    return;
}
