#include "global.h"

#include "field_types_def.h"
#include "map_object.h"

u32 sub_02065CD0(LocalMapObject *param_1, void *param_2);

void sub_020658D4(LocalMapObject *param_1);

void sub_020658D4(LocalMapObject *param_1) {
    u8 *puVar1;

    puVar1 = sub_0205F370(param_1, 0xc);
    sub_02065CD0(param_1, (u8 *)puVar1);
    sub_0205F328(param_1, 0);
    MapObject_ClearSingleMovement(param_1);
    MapObject_ClearFlag18(param_1, 0);
    return;
}
