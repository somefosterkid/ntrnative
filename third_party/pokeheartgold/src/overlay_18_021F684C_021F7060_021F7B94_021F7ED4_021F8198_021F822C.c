#include "global.h"

#include "error_handling.h"
#include "heap.h"

void *ov18_021F8168(u32, u32 *);

void ov18_021F8764(void *, s32 *, void *, s32, void *, u32, u32, void *);

void ov18_021F822C(u32 param_1, void *param_2, s32 *param_3, void *param_4, s32 param_5, void *param_6);

void ov18_021F822C(u32 param_1, void *param_2, s32 *param_3, void *param_4, s32 param_5, void *param_6) {
    u32 uVar1;
    void *unaff_r6;
    u32 uStack_1c;

    uVar1 = 0;
    switch (param_1) {
    case 0:
        memcpy(param_2, param_4, param_5 << 1);
        *param_3 = param_5;
        return;
    case 1:
        unaff_r6 = (void *)ov18_021F8168(2, &uStack_1c);
        uVar1 = 1;
        break;
    case 2:
        unaff_r6 = (void *)ov18_021F8168(3, &uStack_1c);
        break;
    case 3:
        unaff_r6 = (void *)ov18_021F8168(4, &uStack_1c);
        break;
    case 4:
        unaff_r6 = (void *)ov18_021F8168(5, &uStack_1c);
        break;
    case 5:
        unaff_r6 = (void *)ov18_021F8168(6, &uStack_1c);
        break;
    default:
        GF_AssertFail();
    }
    ov18_021F8764(param_2, param_3, param_4, param_5, unaff_r6, uStack_1c, uVar1, param_6);
    Heap_Free(unaff_r6);
    return;
}
