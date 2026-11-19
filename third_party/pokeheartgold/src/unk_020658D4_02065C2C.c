#include "global.h"

#include "field_types_def.h"
#include "map_object.h"

s32 sub_02062428(LocalMapObject *mapObj);

u32 sub_02065C2C(LocalMapObject *param_1, u8 *param_2);

u32 sub_02065C2C(LocalMapObject *param_1, u8 *param_2) {
    s32 iVar1;

    iVar1 = sub_02062428(param_1);
    if (iVar1 == 1) {
        MapObject_ClearSingleMovement(param_1);
        *param_2 = 0;
    }
    return 0;
}
