#include "global.h"

#include "field_types_def.h"
#include "map_object.h"

void sub_02062064(LocalMapObject *param_1, void *param_2);

void sub_02062064(LocalMapObject *param_1, void *param_2) {
    u32 uVar1;

    uVar1 = MapObject_GetType(param_1);
    if (uVar1 == 7) {
        *(u8 *)(param_2 + 1) = 0;
        return;
    }
    *(u8 *)(param_2 + 1) = 1;
    return;
}
