#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "unk_02061284.h"

void MapObject_ForceSetHeldMovement(LocalMapObject *, u32);

s32 sub_02061F5C(LocalMapObject *param_1, s32 param_2, s32 param_3);

u32 sub_0206234C(u32, u32);

void sub_02061720(LocalMapObject *param_1, void *param_2);

u32 sub_02061770(LocalMapObject *param_1, void *param_2);

void sub_02061720(LocalMapObject *param_1, void *param_2) {
    u32 uVar1;
    u32 uVar2;

    uVar1 = sub_02061F5C(param_1, 0x26, -1);
    if (uVar1 == 0xffffffff) {
        uVar1 = MapObject_GetFacingDirection(param_1);
    }
    uVar2 = sub_0206234C(uVar1, 0);
    MapObject_ForceSetHeldMovement(param_1, uVar2);
    *(u8 *)(param_2 + 2) = 1;
    return;
}

s32 sub_02062428(LocalMapObject *mapObj);

u32 sub_02061754(LocalMapObject *param_1, void *param_2) {
    s32 iVar1;

    iVar1 = sub_02062428(param_1);
    if (iVar1 == 0) {
        return 0;
    }
    *(u32 *)((u8 *)param_2 + 4) = 0;
    *(u8 *)((u8 *)param_2 + 2) = 2;
    return 1;
}

u32 sub_02061770(LocalMapObject *param_1, void *param_2) {
    s32 iVar1;

    if ((*(s32 *)(param_2 + 4) != 0) && (iVar1 = sub_02061F5C(param_1, 0x26, -1), iVar1 != -1)) {
        *(u8 *)(param_2 + 2) = 0;
        return 1;
    }
    iVar1 = *(s32 *)(param_2 + 4) + 1;
    *(s32 *)(param_2 + 4) = iVar1;
    if (iVar1 < 0x18) {
        return 0;
    }
    *(u8 *)(param_2 + 2) = 3;
    return 1;
}
