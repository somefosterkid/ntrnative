#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "unk_0205FD20.h"
#include "unk_02061284.h"

void sub_02061648(LocalMapObject *param_1, u32 param_2);

void sub_020616B4(LocalMapObject *param_1) {
    sub_02061648(param_1, 3);
    return;
}

void sub_020616C0(LocalMapObject *param_1, u8 param_2);

void sub_020616C0(LocalMapObject *param_1, u8 param_2) {
    u8 *puVar1;

    puVar1 = sub_0205F370(param_1, 8);
    *puVar1 = param_2;
    sub_0205F328(param_1, 0);
    MapObject_ClearSingleMovement(param_1);
    sub_02060F78(param_1);
    return;
}
