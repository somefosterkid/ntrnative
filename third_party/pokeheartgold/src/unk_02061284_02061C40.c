#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "unk_02061284.h"
#include "error_handling.h"
#include "math_util.h"

void MapObject_ForceSetHeldMovement(LocalMapObject *, u32);

u32 sub_02060BB8(LocalMapObject *, u32);


u32 sub_02062050(LocalMapObject *param_1);

void sub_0206207C(LocalMapObject *param_1, u8 *param_2);

u32 sub_0206234C(u32, u32);

void sub_020620F8(LocalMapObject *param_1, void *param_2);
s32 sub_02062428(LocalMapObject *mapObj);
void sub_02061AEC(LocalMapObject *param_1, u8 param_2, u8 param_3, u32 param_4);


void sub_02061C40(LocalMapObject *param_1, u8 *param_2) {
    u32 uVar1;
    u32 uVar2;
    u32 uVar3;
    u32 uVar4;
    s32 iVar5;
    const s32 *pList;
    s32 iVar6;
    u32 uVar7;
    u32 uVar8;

    if (param_2[1] == param_2[2]) {
        if (param_2[3] == 0) {
            uVar1 = MapObject_GetInitialX(param_1);
            uVar2 = MapObject_GetXCoord(param_1);
            if (uVar1 == uVar2) {
                param_2[1] = param_2[1] + 1;
            }
        } else {
            uVar1 = MapObject_GetInitialZ(param_1);
            uVar2 = MapObject_GetZCoord(param_1);
            if (uVar1 == uVar2) {
                param_2[1] = param_2[1] + 1;
            }
        }
    }
    if (param_2[1] == 3) {
        uVar1 = MapObject_GetInitialX(param_1);
        uVar2 = MapObject_GetInitialZ(param_1);
        uVar3 = MapObject_GetXCoord(param_1);
        uVar4 = MapObject_GetZCoord(param_1);
        if ((uVar1 == uVar3) && (uVar2 == uVar4)) {
            param_2[1] = 0;
        }
    }
    pList = sub_02061E6C(*(s32 *)(param_2 + 4));
    uVar1 = pList[(u8)param_2[1]];
    MapObject_SetNextFacingDirection(param_1, uVar1);
    iVar6 = sub_02062050(param_1);
    if (iVar6 == 0) {
        MapObject_SetFacingDirection(param_1, uVar1);
    }
    uVar7 = sub_02060BB8(param_1, uVar1);
    if ((uVar7 & 1) != 0) {
        param_2[1] = param_2[1] + 1;
        uVar1 = pList[(u8)param_2[1]];
        MapObject_SetNextFacingDirection(param_1, uVar1);
        iVar5 = sub_02062050(param_1);
        if (iVar5 == 0) {
            MapObject_SetFacingDirection(param_1, uVar1);
        }
        uVar7 = sub_02060BB8(param_1, uVar1);
    }
    uVar8 = 0xc;
    if (uVar7 != 0) {
        uVar8 = 0x20;
    }
    uVar8 = sub_0206234C(uVar1, uVar8);
    MapObject_ForceSetHeldMovement(param_1, uVar8);
    iVar5 = sub_02062050(param_1);
    if (iVar5 == 1) {
        sub_0206207C(param_1, param_2 + 8);
    }
    MapObject_SetSingleMovement(param_1);
    *param_2 = 1;
    return;
}

u32 sub_02061D50(LocalMapObject *param_1, u8 *param_2) {
    s32 iVar1;

    iVar1 = sub_02062428(param_1);
    if (iVar1 == 1) {
        MapObject_ClearSingleMovement(param_1);
        iVar1 = sub_02062050(param_1);
        if (iVar1 == 1) {
            sub_020620F8(param_1, (u8 *)(param_2 + 8));
        }
        *param_2 = 0;
    }
    return 0;
}

void sub_02061D80(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 1, 0x1e);
    return;
}

void sub_02061D90(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 1, 0x1f);
    return;
}

void sub_02061DA0(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 0, 0x20);
    return;
}

void sub_02061DB0(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 0, 0x21);
    return;
}

void sub_02061DC0(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 1, 0x22);
    return;
}

void sub_02061DD0(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 1, 0x23);
    return;
}

void sub_02061DE0(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 0, 0x24);
    return;
}

void sub_02061DF0(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 0, 0x25);
    return;
}

s32 sub_02061E00(const s32 *param_1, s32 param_2) {
    s32 iVar1;
    s32 iVar2;

    iVar2 = 0;
    iVar1 = *param_1;
    while (param_2 != iVar1) {
        param_1 = param_1 + 1;
        iVar2 = iVar2 + 1;
        iVar1 = *param_1;
    }
    if (iVar2 == 0) {
        GF_AssertFail();
    }
    return iVar2;
}

s32 sub_02061E20(const s32 *param_1, s32 param_2) {
    u16 uVar1;
    s32 iVar2;
    s32 remainder;

    uVar1 = LCRandom();
    iVar2 = sub_02061E00(param_1, param_2);
    remainder = (s32)(uVar1) % (s32)(iVar2);
    return param_1[remainder];
}

s32 sub_02061E44(s32 param_1, s32 param_2) {
    u16 uVar1;
    const s32 *piVar2;
    s32 iVar3;
    s32 remainder;

    piVar2 = sub_02061E6C(param_1);
    uVar1 = LCRandom();
    iVar3 = sub_02061E00(piVar2, param_2);
    remainder = (s32)(uVar1) % (s32)(iVar3);
    return piVar2[remainder];
}

const s32 *sub_02061E6C(s32 param_1) {
    const MapObjectDirectionPattern *pPattern;
    s32 iVar2;

    pPattern = _020FD838;
    iVar2 = 0;
    do {
        if (iVar2 == param_1) {
            return pPattern->directions;
        }
        pPattern = pPattern + 1;
        iVar2 = pPattern->id;
    } while (iVar2 != 0x27);
    GF_AssertFail();
    return NULL;
}
