#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "unk_0205FD20.h"
#include "unk_02061284.h"

s32 MapObject_ForceSetHeldMovement(LocalMapObject *, s32);

s32 sub_02060BB8(LocalMapObject *, u32);

s32 sub_02062050(LocalMapObject *);

s32 sub_0206207C(LocalMapObject *, void *);

s32 sub_0206234C(u32, s32);

s32 sub_020619FC(LocalMapObject *arg0, void *arg1);

s32 sub_020619FC(LocalMapObject *object, void *arg1) {
    u32 v0;
    s32 v4;
    s32 v5;
    u32 v1;
    u32 direction;
    u32 v2;
    u32 v3;
    u32 v6;

    if (*(s16 *)((u8 *)arg1 + 2) != 0) {
        v3 = MapObject_GetInitialX(object);
        v0 = MapObject_GetInitialZ(object);
        v2 = MapObject_GetXCoord(object);
        v1 = MapObject_GetZCoord(object);
        if ((v3 == v2) && (v0 == v1)) {
            direction = sub_020611F4(MapObject_GetNextFacingDirection(object));
            MapObject_SetNextFacingDirection(object, direction);
            if (sub_02062050(object) == 0) {
                MapObject_SetFacingDirection(object, direction);
            }
            *(s16 *)((u8 *)arg1 + 2) = 0;
        }
    }
    v6 = MapObject_GetNextFacingDirection(object);
    v4 = sub_02060BB8(object, v6);
    if (v4 & 1) {
        *(s16 *)((u8 *)arg1 + 2) = 1;
        v6 = sub_020611F4(v6);
        v4 = sub_02060BB8(object, v6);
    }
    v5 = 0xC;
    if (v4 != 0) {
        v5 = 0x20;
    }
    MapObject_ForceSetHeldMovement(object, sub_0206234C(v6, v5));
    if (sub_02062050(object) == 1) {
        sub_0206207C(object, arg1 + 4);
    }
    MapObject_SetSingleMovement(object);
    *(s16 *)arg1 = 2;
    return 1;
}

void sub_020620F8(LocalMapObject *param_1, void *param_2);

s32 sub_02062428(LocalMapObject *mapObj);

u32 sub_02061ABC(LocalMapObject *param_1, u16 *param_2);

u32 sub_02061ABC(LocalMapObject *param_1, u16 *param_2) {
    s32 iVar1;

    iVar1 = sub_02062428(param_1);
    if (iVar1 == 1) {
        MapObject_ClearSingleMovement(param_1);
        iVar1 = sub_02062050(param_1);
        if (iVar1 == 1) {
            sub_020620F8(param_1, (u8 *)(param_2 + 2));
        }
        *param_2 = 0;
    }
    return 0;
}

void sub_02062064(LocalMapObject *param_1, void *param_2);

void sub_02061AEC(LocalMapObject *param_1, u8 param_2, u8 param_3, u32 param_4);

void sub_02061AEC(LocalMapObject *param_1, u8 param_2, u8 param_3, u32 param_4) {
    u8 *puVar1;
    s32 iVar2;

    puVar1 = sub_0205F370(param_1, 0xc);
    puVar1[2] = param_2;
    puVar1[3] = param_3;
    *(u32 *)(puVar1 + 4) = param_4;
    iVar2 = sub_02062050(param_1);
    if (iVar2 == 1) {
        sub_02062064(param_1, (puVar1 + 8));
    }
    return;
}

void sub_02061B1C(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 0, 0xe);
    return;
}

void sub_02061B2C(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 0, 0xf);
    return;
}

void sub_02061B3C(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 1, 0x10);
    return;
}

void sub_02061B4C(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 1, 0x11);
    return;
}

void sub_02061B5C(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 0, 0x12);
    return;
}

void sub_02061B6C(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 0, 0x13);
    return;
}

void sub_02061B7C(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 1, 0x14);
    return;
}

void sub_02061B8C(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 1, 0x15);
    return;
}

void sub_02061B9C(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 1, 0x16);
    return;
}

void sub_02061BAC(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 1, 0x17);
    return;
}

void sub_02061BBC(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 0, 0x18);
    return;
}

void sub_02061BCC(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 0, 0x19);
    return;
}

void sub_02061BDC(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 1, 0x1a);
    return;
}

void sub_02061BEC(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 1, 0x1b);
    return;
}

void sub_02061BFC(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 0, 0x1c);
    return;
}

void sub_02061C0C(LocalMapObject *param_1) {
    sub_02061AEC(param_1, 2, 0, 0x1d);
    return;
}
