#include "global.h"

#include "constants/heap.h"

#include "save.h"
#include "unk_02037C94.h"

void ov65_0221E330(s32 param_1, u32 param_2);

u32 ov65_0221FC54(u32, s32, s32, s32, s32 arg4);

s32 sub_0202C2DC(WiFiList *, s32);

void ov65_0221E600();

void ov65_0221E740();

u32 ov65_0221E684(s32 param_1, u32 param_2, u32 param_3, u32 param_4);

u32 ov65_0221E684(s32 param_1, u32 param_2, u32 param_3, u32 param_4) {
    u32 uVar1;
    s32 iVar2;
    s32 iVar3;
    s32 iVar4;
    s32 arg2;

    iVar3 = param_1 + 0x670;
    iVar4 = param_1 + 0x36c0;
    uVar1 = ov65_0221FC54(*(u32 *)(param_1 + 0x180), param_1 + 0x664, iVar3, iVar4, 1);
    switch (uVar1) {
    case 1:
    case 3:
        *(void (**)())(param_1 + 0x2220) = ov65_0221E740;
        arg2 = 0;
        do {
            iVar2 = sub_0202C2DC(*(void **)(param_1 + 0x36a0), arg2);
            if (iVar2 == 0) {
                iVar3 = arg2;
                sub_0203A280(*(SaveData **)(param_1 + 4), *(s32 *)(param_1 + 0x3678), arg2, HEAP_ID_26, 0);
                iVar4 = 0x1a;
                break;
            }
            arg2 = arg2 + 1;
        } while (arg2 < 0x20);
        if (arg2 == 0x20) {
            ov65_0221E330(param_1, 0x3f);
            *(void (**)())(param_1 + 0x2220) = ov65_0221E600;
            return 0;
        }
        break;
    case 2:
    case 4:
        *(void (**)())(param_1 + 0x2220) = ov65_0221E740;
    }
    return 0;
}
