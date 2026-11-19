#include "global.h"

#include "field_types_def.h"
#include "map_object.h"

s32 MapObject_RunMovementCommand(LocalMapObject *, u32, u32);

void sub_02062400(LocalMapObject *param_1);

void sub_02062400(LocalMapObject *param_1) {
    u32 uVar1;
    u32 uVar2;
    s32 iVar3;

    do {
        uVar1 = MapObject_GetMovementCommand(param_1);
        if (uVar1 == 0xff) {
            return;
        }
        uVar2 = MapObject_GetMovementStep(param_1);
        iVar3 = MapObject_RunMovementCommand(param_1, uVar1, uVar2);
    } while (iVar3 != 0);
    return;
}
