#include "global.h"

#include "error_handling.h"
#include "overlay_01_rodata_02209704.h"

typedef struct Words4 {
    u32 w[4];
} Words4;

s32 ov01_0220542C(s32 param_1, s32 param_2);

s32 ov01_0220542C(s32 param_1, s32 param_2) {
    u32 uVar1;
    s32 aiStack_20[4];
    s32 aiStack_30[4];
    s32 aiStack_40[4];
    s32 aiStack_50[4];

    *(Words4 *)((u8 *)aiStack_20) = *(Words4 *)ov01_02209704.tables[1];
    *(Words4 *)((u8 *)aiStack_30) = *(Words4 *)ov01_02209704.tables[2];
    *(Words4 *)((u8 *)aiStack_40) = *(Words4 *)ov01_02209704.tables[3];
    *(Words4 *)((u8 *)aiStack_50) = *(Words4 *)ov01_02209704.tables[0];
    uVar1 = 0;
    do {
        if (param_2 == aiStack_20[uVar1]) {
            return aiStack_30[param_1];
        }
        uVar1 = uVar1 + 1 & 0xff;
    } while (uVar1 < 4);
    uVar1 = 0;
    do {
        if (param_2 == aiStack_30[uVar1]) {
            return aiStack_40[param_1];
        }
        uVar1 = uVar1 + 1 & 0xff;
    } while (uVar1 < 4);
    uVar1 = 0;
    do {
        if (param_2 == aiStack_40[uVar1]) {
            return aiStack_50[param_1];
        }
        uVar1 = uVar1 + 1 & 0xff;
    } while (uVar1 < 4);
    GF_AssertFail();
    return 0;
}
