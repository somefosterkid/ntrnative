#include "global.h"

#include "unk_02031B0C.h"

s32 sub_02031D6C(ApricornJuice *arg0);

s32 sub_02031D6C(ApricornJuice *a0) {
    if (sub_02031D80(a0) != 0) {
        return 1;
    }
    return 0;
}

int sub_02031D80(ApricornJuice *a0) {
    u8 *puVar1;
    s32 iVar2;
    u32 uVar3;

    uVar3 = 0;
    iVar2 = 0;
    do {
        puVar1 = &a0->flavors[iVar2];
        uVar3 = uVar3 + *puVar1 & 0xffff;
        iVar2 = iVar2 + 1;
    } while (iVar2 < 5);
    if (100 < uVar3) {
        uVar3 = 100;
    }
    return uVar3 & 0xff;
}
