#include "global.h"

#include "field_types_def.h"
#include "follow_mon.h"
#include "map_object.h"

void MapObject_ForceSetHeldMovement(LocalMapObject *, u32);

BOOL sub_02062428(LocalMapObject *);

u32 sub_02065C48(LocalMapObject *param_1, u8 *param_2);

u32 sub_02065C48(LocalMapObject *param_1, u8 *param_2) {
    u8 uVar1;
    s32 iVar2;
    iVar2 = sub_02062428(param_1);
    if (iVar2 == 1) {
        param_2[2] = param_2[2] + 1;
        if (2 <= ((u8)param_2[2])) {
            MapObject_ClearSingleMovement(param_1);
            *param_2 = 0;
            param_2[3] = 0;
            uVar1 = 0xfff9;
            *((u16 *)(param_2 + 10)) = (*((u16 *)(param_2 + 10))) & ((char)uVar1);
            return 0;
        }
        uVar1 = sub_02069ED4(param_1);
        MapObject_ForceSetHeldMovement(param_1, uVar1);
    }
    param_2[3] = param_2[3] + 1;
    return 0;
}
