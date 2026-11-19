#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"

void _s32_div_f(s32, u32);

void ov96_02219DA8(PokeathlonCourseData *param_1, enum HeapID param_2, ManagedSprite *param_3, s32 param_4, u8 param_5);

void ov96_02219EE0(void *param_1, s32 param_2);

void ov96_02219F20(u8 *param_1, s32 param_2, s16 param_3, s16 param_4);

u32 ov96_02219794(u32 *param_1, u32 param_2, u8 param_3);

u32 ov96_02219794(u32 *param_1, u32 param_2, u8 param_3) {
    s32 cVar1;
    u8 uVar2;
    u8 remainder;
    s32 iVar3;
    u32 uVar4;
    s16 sStack_1a[3];

    uVar2 = ov96_021E5F24((PokeathlonCourseData *)*param_1);
    remainder = (s32)(param_3 + 1) % (s32)(3);
    cVar1 = *(u8 *)(param_1 + 0x30);
    switch (cVar1) {
    case 0:
        ManagedSprite_GetPositionXYWithSubscreenOffset((ManagedSprite *)param_1[0x27], &sStack_1a[2], &sStack_1a[1], FX32_CONST(480));
        if (sStack_1a[1] < 0xd8) {
            s32 d0;
            d0 = 0x110 - sStack_1a[1];
            if (d0 < 0) {
                d0 = -d0;
            }
            sStack_1a[1] += (s16)(d0 >> 2);
            if (0xd8 < sStack_1a[1]) {
                sStack_1a[1] = 0xd8;
            }
            ov96_02219F20((u8 *)param_1, 0xf, sStack_1a[2], sStack_1a[1]);
        } else {
            sStack_1a[2] = 0x28;
            sStack_1a[1] = -0x28;
            ov96_02219F20((u8 *)param_1, 0xf, 0x28, -0x28);
            ov96_02219DA8((PokeathlonCourseData *)*param_1, (enum HeapID)param_1[1], (ManagedSprite *)param_1[0x27], (u8)uVar2, param_3);
            ov96_02219EE0((u8 *)param_1, (u8)remainder);
            *(u8 *)(param_1 + 0x30) = *(u8 *)(param_1 + 0x30) + 1;
        }
        break;
    case 1:
        s16 sStack_1c;
        ManagedSprite_GetPositionXYWithSubscreenOffset((ManagedSprite *)param_1[0x26], &sStack_1a[0], &sStack_1c, FX32_CONST(480));
        iVar3 = 0x88 - sStack_1c;
        if (iVar3 < 0) {
            iVar3 = -iVar3;
        }
        iVar3 = (s32)((iVar3 * 3 + ((u32)(iVar3 * 3 >> 1) >> 0x1e)) * 0x4000) >> 0x10;
        if (sStack_1c <= 0x88) {
            if (iVar3 == 0) {
                iVar3 = 1;
            }
            sStack_1c += iVar3;
            ov96_02219F20((u8 *)param_1, 0xe, sStack_1a[0], sStack_1c);
            ManagedSprite_GetPositionXYWithSubscreenOffset((ManagedSprite *)param_1[0x27], &sStack_1a[0], &sStack_1c, FX32_CONST(480));
            sStack_1c += iVar3;
            ov96_02219F20((u8 *)param_1, 0xf, sStack_1a[0], sStack_1c);
        } else {
            uVar4 = param_1[0x26];
            param_1[0x26] = param_1[0x27];
            param_1[0x27] = uVar4;
            uVar4 = param_1[0x28];
            param_1[0x28] = param_1[0x29];
            param_1[0x29] = uVar4;
            ov96_02219F20((u8 *)param_1, 0xe, 0x28, 0x30);
            ov96_02219F20((u8 *)param_1, 0xf, 0x28, 0x88);
            *(u8 *)(param_1 + 0x30) = *(u8 *)(param_1 + 0x30) + 1;
        }
        break;
    case 2:
        *(u8 *)(param_1 + 0x30) = 0;
        return 1;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}
