#include "global.h"

#include "error_handling.h"
#include "math_util.h"
#include "sprite_system.h"

s32 ov96_021ED6E8(void);

s32 ov96_021ECF48(u32 *);

void ov96_021ECFF8(u32 *, u32, u32);

void ov96_021ED17C(u32 *, s32);

void ov96_021ED0C8(u32 *param_1);

void ov96_021ED0C8(u32 *param_1) {
    u32 uVar1;
    u32 uVar2;
    s32 iVar3;
    u32 *puVar4;
    u32 *puVar5;
    s32 iVar6;
    if (param_1 == 0L) {
        GF_AssertFail();
    }
    if ((param_1[0x5f] != 0) && (uVar1 = param_1[0x5d], param_1[0x5d] = param_1[0x5d] + 1, uVar1 > param_1[0x5e])) {
        uVar2 = MTRandom();
        ov96_021ED17C(param_1, (u8)((uVar2 & 1) + 2));
        uVar1 = ov96_021ED6E8();
        param_1[0x5e] = uVar1;
        param_1[0x5d] = 0;
    }
    iVar6 = 0;
    puVar5 = param_1 + 6;
    puVar4 = param_1 + 1;
    do {
        if ((((*puVar5 << 11) >> 31) == 1) && (iVar3 = ov96_021ECF48(puVar4), iVar3 != 0)) {
            ov96_021ECFF8(puVar4, param_1[0x5b], param_1[0x5c]);
        }
        iVar6 = iVar6 + 1;
        puVar5 = puVar5 + 6;
        puVar4 = puVar4 + 6;
    } while (iVar6 < 0xf);
    return;
}
