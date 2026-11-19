#include "global.h"

#include "frontier/overlay_80_022340E8.h"

#include "party.h"
#include "pokemon_types_def.h"

void ov83_02246114(void *, u32);

u32 ov83_02247454(void *, u32, u32, u32, u32, s32, u32, u32, void *);

void ov83_0224753C(void *);

s32 ov83_02247624(void *);

u8 ov83_02247768(u8, u32, u32);

u32 ov83_02244E24(void *param_1, u32 param_2, u32 param_3);

u32 ov83_02244E24(void *param_1, u32 param_2, u32 param_3) {
    s32 iVar1;
    s32 iVar2;
    u32 uVar3;

    iVar1 = ov83_02247768(*(u8 *)(param_1 + 0x14), param_2, param_3);
    Party_GetMonByIndex(*(Party **)(param_1 + 0x55c), iVar1);
    switch (param_3) {
    case 0:
        *(u8 *)(param_1 + 0xf) = *(u8 *)(param_1 + 0xf) & ~1;
        return 1;
    case 1:
        if (((u32)(*(u8 *)(param_1 + 0xf) << 0x1f) >> 0x1f) == 0) {
            *(u8 *)(param_1 + 0xf) = *(u8 *)(param_1 + 0xf) & ~1 | 1;
            if (*(u8 *)(param_1 + 0x12) == 1) {
                uVar3 = 9;
            } else {
                uVar3 = 10;
            }
            iVar2 = ov80_02237D8C(*(u8 *)(param_1 + 9));
            if (iVar2 == 0) {
                iVar2 = 0x50;
            } else {
                iVar2 = 0x30;
            }
            uVar3 = ov83_02247454(param_1 + 0x2c8, 0, 0, 0, uVar3, (iVar2 + iVar1 * 0x40) * 0x10000 >> 0x10, 0x32, 0, 0);
            *(u32 *)(param_1 + 0x504) = uVar3;
        }
        iVar1 = ov83_02247624(*(void **)(param_1 + 0x504));
        if (iVar1 == 0) {
            ov83_0224753C(*(void **)(param_1 + 0x504));
            *(u32 *)(param_1 + 0x504) = 0;
            *(u8 *)(param_1 + 0xf) = *(u8 *)(param_1 + 0xf) & ~1;
            return 1;
        }
        break;
    case 3:
        iVar1 = ov80_02237D8C(*(u8 *)(param_1 + 9));
        if (iVar1 == 0) {
            ov83_02246114(param_1, 0);
            *(u8 *)(param_1 + 0xf) = *(u8 *)(param_1 + 0xf) & ~1;
            return 1;
        }
        *(u8 *)(param_1 + 0xf) = *(u8 *)(param_1 + 0xf) & ~1;
        return 1;
    case 4:
        iVar1 = ov80_02237D8C(*(u8 *)(param_1 + 9));
        if (iVar1 == 0) {
            ov83_02246114(param_1, 0);
            *(u8 *)(param_1 + 0xf) = *(u8 *)(param_1 + 0xf) & ~1;
            return 1;
        }
        *(u8 *)(param_1 + 0xf) = *(u8 *)(param_1 + 0xf) & ~1;
        return 1;
    case 5:
        *(u8 *)(param_1 + 0xf) = *(u8 *)(param_1 + 0xf) & ~1;
        return 1;
    }
    return 0;
}
