#include "global.h"

#include "unk_02005D10.h"
#include "unk_02035900.h"

void ov83_02245390(void *arg0);

void ov83_02245824(void *arg0, s32 arg1);

void ov80_02237FA4(void *, u8, u32);

void ov83_02244ABC(void *, u32);

u8 ov83_02245068(u8);

void ov83_02245288(void *, u32);

void ov83_02245318(void *, u32);

void ov83_022453DC(void *, void *);

void ov83_02245838(void *, u32, s32);

u32 ov83_0224776C(u32, s32 arg1);

void ov83_022477C4(void *, u32);

void ov83_0224563C(void *param_1, u32 param_2, u32 param_3);

void ov83_0224563C(void *param_1, u32 param_2, u32 param_3) {
    u16 uVar1;
    s32 uVar2;
    u32 uVar3;
    u8 unaff_r5;

    PlaySE(0x5e3);
    uVar2 = (u32) * (u8 *)((u8 *)param_1 + 0x15);
    uVar3 = ov83_0224776C(uVar2, param_2);
    switch (param_3) {
    case 0:
        unaff_r5 = 1;
        break;
    case 1:
        unaff_r5 = ov83_02245068(*(u8 *)((u8 *)param_1 + 0x12));
        break;
    case 3:
        unaff_r5 = 2;
        break;
    case 4:
        unaff_r5 = 5;
    }
    uVar1 = sub_0203769C();
    if (uVar1 == 0) {
        if (param_2 < uVar2) {
            ov83_02244ABC(param_1, 5);
            ov80_02237FA4(*(void **)((u8 *)param_1 + 4), *(u8 *)((u8 *)param_1 + 9), unaff_r5);
        } else {
            ov83_022477C4(*(void **)((u8 *)param_1 + 0x24), 5);
            *(s16 *)((u8 *)param_1 + 0x5ba) = *(u16 *)((u8 *)param_1 + 0x5ba) - (s16)unaff_r5;
        }
    } else if (param_2 < uVar2) {
        ov83_022477C4(*(void **)((u8 *)param_1 + 0x24), 5);
        *(s16 *)((u8 *)param_1 + 0x5ba) = *(u16 *)((u8 *)param_1 + 0x5ba) - (s16)unaff_r5;
    } else {
        ov83_02244ABC(param_1, 5);
        ov80_02237FA4(*(void **)((u8 *)param_1 + 4), *(u8 *)((u8 *)param_1 + 9), unaff_r5);
    }
    ov83_022453DC(param_1, (u8 *)param_1 + 0x50);
    ov83_02245390(param_1);
    switch (param_3) {
    case 0:
        ov83_02245824(param_1, uVar3);
        return;
    case 1:
        ov83_02245838(param_1, uVar3, *(u8 *)((u8 *)param_1 + 0x12));
        return;
    case 3:
        ov83_02245288(param_1, uVar3);
        return;
    case 4:
        ov83_02245318(param_1, uVar3);
    }
    return;
}
