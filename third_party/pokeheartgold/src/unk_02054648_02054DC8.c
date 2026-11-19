#include "global.h"

void sub_02054DC8(const int mapMatrixIndex, const int mapMatrixWidth, VecFx32 *mapAbsoluteOrigin);

extern u32 _020FC60C;
u32 sub_02054E00(u32 param_1);

void sub_02054DC8(const int mapMatrixIndex, const int mapMatrixWidth, VecFx32 *mapAbsoluteOrigin) {
    mapAbsoluteOrigin->x = (32 / 2) * (16 * ((fx32)0x0000000000001000L));
    mapAbsoluteOrigin->z = (32 / 2) * (16 * ((fx32)0x0000000000001000L));

    u16 mapMatrixX = mapMatrixIndex % mapMatrixWidth;
    u16 mapMatrixZ = mapMatrixIndex / mapMatrixWidth;

    mapAbsoluteOrigin->x += mapMatrixX * 32 * (16 * ((fx32)0x0000000000001000L));
    mapAbsoluteOrigin->z += mapMatrixZ * 32 * (16 * ((fx32)0x0000000000001000L));
}

u32 sub_02054E00(u32 param_1) {
    s32 iVar1;
    u16 *puVar2;

    puVar2 = (u16 *)&_020FC60C;
    iVar1 = 0;
    do {
        if (param_1 == *puVar2) {
            return 1;
        }
        iVar1 = iVar1 + 1;
        puVar2 = puVar2 + 1;
    } while (iVar1 < 4);
    return 0;
}
