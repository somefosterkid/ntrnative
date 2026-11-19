#include "global.h"

extern u32 ov70_022459F4;

extern u32 ov70_02245A4C;

s32 ov70_0223F864(s32 param_1, s32 param_2, s32 param_3);

s32 ov70_0223F864(s32 param_1, s32 param_2, s32 param_3) {
    s32 iVar1;
    s32 iVar2;
    u8 *puVar3;

    if (param_3 == 0) {
        puVar3 = (unsigned char *)(&ov70_02245A4C);
        iVar2 = 0xc;
    } else {
        puVar3 = (unsigned char *)(&ov70_022459F4);
        iVar2 = 0xb;
    }
    iVar1 = 0;
    while (iVar1 < iVar2) {
        if ((param_1 == *(s16 *)(puVar3 + 4)) && (param_2 == *(s16 *)(puVar3 + 6))) {
            return iVar1;
        }
        iVar1 = iVar1 + 1;
        puVar3 = puVar3 + 8;
    }
    return 0;
}
