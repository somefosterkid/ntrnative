#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "unk_02061284.h"

void sub_020613D0(LocalMapObject *param_1, u32 param_2, u32 param_3, u32 param_4);

void MapObject_ForceSetHeldMovement(LocalMapObject *, u32);
s32 sub_02060BB8(LocalMapObject *, u32);
u32 sub_020615F0(LocalMapObject *param_1, s32 param_2);
u32 sub_0206234C(u32, u32);
s32 sub_02062428(LocalMapObject *);

void sub_020613B0(LocalMapObject *param_1) {
    sub_020613D0(param_1, 0xc, 0xc, 0);
    return;
}

void sub_020613C0(LocalMapObject *param_1) {
    sub_020613D0(param_1, 0xc, 0xd, 0);
    return;
}

void sub_020613D0(LocalMapObject *param_1, u32 param_2, u32 param_3, u32 param_4) {
    u8 *puVar1;

    puVar1 = sub_0205F370(param_1, 0x10);
    *(u32 *)(puVar1 + 4) = param_4;
    *(u32 *)(puVar1 + 8) = param_2;
    *(u32 *)(puVar1 + 0xc) = param_3;
    sub_0205F328(param_1, 0);
    MapObject_ClearSingleMovement(param_1);
    return;
}

void sub_020613F8(LocalMapObject *param_1) {
    s16 *psVar1;
    u32 uVar2;
    u32 uVar3;
    s32 iVar4;

    psVar1 = (s16 *)sub_0205F394(param_1);
    switch (*psVar1) {
    case 0:
        MapObject_ClearSingleMovement(param_1);
        MapObject_ClearEndMovement(param_1);
        uVar2 = MapObject_GetFacingDirection(param_1);
        uVar3 = sub_0206234C(uVar2, 0);
        MapObject_ForceSetHeldMovement(param_1, uVar3);
        *psVar1 = *psVar1 + 1;
        return;
    case 1:
        iVar4 = sub_02062428(param_1);
        if (iVar4 == 0) {
            return;
        }
        iVar4 = sub_02061E20(_020FD7B8, -1);
        psVar1[1] = (s16)iVar4;
        *psVar1 = *psVar1 + 1;
    case 2:
        psVar1[1] = psVar1[1] + -1;
        if (psVar1[1] == 0) {
            *psVar1 = *psVar1 + 1;
code_r0x020001fa:
            uVar2 = sub_02061E44(*(u32 *)(psVar1 + 6), -1);
            MapObject_SetOrQueueFacing(param_1, uVar2);
            if ((*(s32 *)(psVar1 + 2) == 1) && (iVar4 = sub_020615F0(param_1, uVar2), iVar4 == 0)) {
                *psVar1 = 0;
                return;
            }
            iVar4 = sub_02060BB8(param_1, uVar2);
            if (iVar4 != 0) {
                *psVar1 = 0;
                return;
            }
            uVar3 = sub_0206234C(uVar2, *(u32 *)(psVar1 + 4));
            MapObject_ForceSetHeldMovement(param_1, uVar3);
            MapObject_SetSingleMovement(param_1);
            *psVar1 = *psVar1 + 1;
            goto code_r0x02000256;
        }
        break;
    case 3:
        goto code_r0x020001fa;
    case 4:
code_r0x02000256:
        iVar4 = sub_02062428(param_1);
        if (iVar4 != 0) {
            MapObject_ClearSingleMovement(param_1);
            *psVar1 = 0;
        }
        break;
    default:
        break;
    }
    return;
}
