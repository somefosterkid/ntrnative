#include "global.h"

#include "field_types_def.h"
#include "map_object.h"

u32 sub_02066150(LocalMapObject *param_1, void *param_2);

void sub_02065FFC(LocalMapObject *param_1);

void sub_02065FFC(LocalMapObject *param_1) {
    u8 *puVar1;

    puVar1 = sub_0205F370(param_1, 0xc);
    sub_02066150(param_1, (u8 *)puVar1);
    sub_0205F328(param_1, 0);
    MapObject_ClearSingleMovement(param_1);
    puVar1[1] = 0;
    return;
}
