#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "pokedex.h"

void *ov18_021F8168(u32, u32 *);

void ov18_021F8764(void *param_1, s32 *param_2, u16 *param_3, u32 param_4, u16 *param_5, u32 param_6, s32 param_7, Pokedex *param_8);

void ov18_021F8468(u32 param_1, void *param_2, s32 *param_3, void *param_4, s32 param_5, Pokedex *param_6);

void ov18_021F8468(u32 param_1, void *param_2, s32 *param_3, void *param_4, s32 param_5, Pokedex *param_6) {
    u16 *unaff_r7;
    u32 uStack_18;

    switch (param_1) {
    case 0xe:
        memcpy(param_2, param_4, param_5 << 1);
        *param_3 = param_5;
        return;
    case 7:
        unaff_r7 = (u16 *)ov18_021F8168(0x44, &uStack_18);
        break;
    case 0xd:
        unaff_r7 = (u16 *)ov18_021F8168(0x45, &uStack_18);
        break;
    case 0xb:
        unaff_r7 = (u16 *)ov18_021F8168(0x46, &uStack_18);
        break;
    case 5:
        unaff_r7 = (u16 *)ov18_021F8168(0x47, &uStack_18);
        break;
    case 4:
        unaff_r7 = (u16 *)ov18_021F8168(0x48, &uStack_18);
        break;
    case 0xc:
        unaff_r7 = (u16 *)ov18_021F8168(0x49, &uStack_18);
        break;
    case 9:
        unaff_r7 = (u16 *)ov18_021F8168(0x4a, &uStack_18);
        break;
    case 6:
        unaff_r7 = (u16 *)ov18_021F8168(0x4b, &uStack_18);
        break;
    case 1:
        unaff_r7 = (u16 *)ov18_021F8168(0x4c, &uStack_18);
        break;
    case 2:
        unaff_r7 = (u16 *)ov18_021F8168(0x4d, &uStack_18);
        break;
    case 3:
        unaff_r7 = (u16 *)ov18_021F8168(0x4e, &uStack_18);
        break;
    case 10:
        unaff_r7 = (u16 *)ov18_021F8168(0x4f, &uStack_18);
        break;
    case 0:
        unaff_r7 = (u16 *)ov18_021F8168(0x50, &uStack_18);
        break;
    case 8:
        unaff_r7 = (u16 *)ov18_021F8168(0x51, &uStack_18);
        break;
    default:
        GF_AssertFail();
    }
    ov18_021F8764(param_2, param_3, unaff_r7, uStack_18, param_4, param_5, 1, param_6);
    Heap_Free(unaff_r7);
    return;
}
