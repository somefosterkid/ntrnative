#include "global.h"

s32 ov40_0222BF80(s32, s32);

s32 ov40_02230944(s32);

s32 ov40_0223CE38(s32);

void ov40_0223CF70(s32 arg0, s32 arg1, s32 arg2);

void ov39_022271C0(void *, void *);
void *sub_02030920(u32);
void *sub_020314A4(u32);
void ov40_0223CFA8(u8 *param_1);

void ov40_0223CF70(s32 arg0, s32 arg1, s32 arg2) {
    if (arg1 == 0) {
        switch (arg0) {
        case 0:
            ov40_02230944(arg2);
            ov40_0223CE38(arg2);
            ov40_0222BF80(arg2, 0x11);
            return;
        case 1:
            ov40_02230944(arg2);
            ov40_0222BF80(arg2, 0x10);
            break;
        }
    }
}

void ov40_0223CFA8(u8 *param_1) {
    void *uVar1;
    u8 *iVar2;

    iVar2 = *(u8 **)(param_1 + 0x860);
    uVar1 = sub_020314A4(0x6d);
    *(void **)(iVar2 + 0x194) = uVar1;
    ov39_022271C0(*(void **)(param_1 + *(s32 *)(param_1 + 0x4d4) * 4 + 0x2608),
        *(void **)(iVar2 + 0x194));
    *(u32 *)(iVar2 + 0x220) = *(u32 *)(iVar2 + 0x194);
    uVar1 = sub_02030920(0x6d);
    *(void **)(iVar2 + 0x224) = uVar1;
    MI_CpuCopy8((void *)(*(u8 **)(param_1 + *(s32 *)(param_1 + 0x4d4) * 4 + 0x2608) + 0x80),
        *(void **)(iVar2 + 0x224),
        100);
    return;
}
