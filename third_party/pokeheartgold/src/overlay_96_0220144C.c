#include "global.h"
#include "math_util.h"

void ov96_0220144C(s32 *arg0, s32 arg1);

s8 ov96_022014A4(void *param_1, void *param_2);
u32 ov96_0220146C(s32 param_1);

void ov96_0220144C(s32 *arg0, s32 arg1) {
    s32 v0;

    v0 = *arg0 + arg1;
    *arg0 = v0;
    if (v0 >= 0x400) {
        *arg0 = v0 - 0x400;
        return;
    }
    if (v0 < 0) {
        *arg0 = v0 + 0x400;
    }
}

u32 ov96_0220146C(s32 param_1) {
    s8 cVar1;
    u16 uVar2;
    s32 iVar3;
    s32 *piVar4;
    u32 remainder;
    s32 iVar5;
    u32 auStack_48[16];
    s32 aiStack_88[16];

    iVar3 = param_1 + 0x29c;
    iVar5 = 0;
    piVar4 = aiStack_88;
    do {
        iVar5 = iVar5 + 1;
        *piVar4 = iVar3;
        piVar4 = piVar4 + 1;
        iVar3 = iVar3 + 0x14;
    } while (iVar5 < 0x10);
    cVar1 = ov96_022014A4((u8 *)aiStack_88, (u8 *)auStack_48);
    uVar2 = LCRandom();
    remainder = (s32)(uVar2) % (s32)((s32)cVar1);
    return auStack_48[remainder & 0xff];
}
