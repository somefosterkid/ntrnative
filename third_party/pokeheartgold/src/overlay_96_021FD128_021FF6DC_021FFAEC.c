#include "global.h"

#include "error_handling.h"

void ov96_021EB588(void *param_1, VecFx32 *param_2);

void ov96_021FFAEC(void *param_1, fx32 *param_2, void *param_3, s32 arg3);

void ov96_021FFAEC(void *param_1, fx32 *param_2, void *param_3, s32 arg3) {
    u8 cVar1;
    VecFx32 VStack_18;

    VStack_18 = *(VecFx32 *)((u8 *)param_2);
    cVar1 = *(s8 *)(param_1 + 8);
    switch (cVar1) {
    case 1:
        VStack_18.y = VStack_18.y + -0x14000;
        break;
    case 2:
        VStack_18.y = VStack_18.y + -0x20000;
        break;
    case 3:
        VStack_18.y = VStack_18.y + -0x28000;
        break;
    default:
        GF_AssertFail();
        break;
    }
    ov96_021EB588(param_3, &VStack_18);
    return;
}
