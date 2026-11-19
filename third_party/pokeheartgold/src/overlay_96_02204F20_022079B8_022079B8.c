#include "global.h"

#include "fx_util.h"

extern VecFx32 ov96_0221CAC8;

u32 ov96_022079B8(VecFx32 *param_1);

u32 ov96_022079B8(VecFx32 *param_1) {
    fx32 fVar1;
    u32 uVar2;
    u32 uVar3;
    VecFx32 VStack_18;
    VecFx32 VStack_24;
    VecFx32 VStack_30;

    uVar3 = 1;
    VEC_Normalize(param_1, &VStack_18);
    fVar1 = VEC_Mag(&VStack_18);
    if (fVar1 != 0) {
        VStack_24 = ov96_0221CAC8;
        VStack_30.x = VStack_18.y;
        VStack_30.z = VStack_18.x;
        VStack_30.y = 0;
        uVar2 = VEC_AngleBetween(&VStack_24, &VStack_30);
        if ((uVar2 <= 0x2000) || (uVar2 >= 0xE000)) {
            uVar3 = 4;
        } else if ((uVar2 > 0x2000) && (uVar2 < 0x6000)) {
            uVar3 = 2;
        } else if ((uVar2 >= 0x6000) && (uVar2 <= 0xa000)) {
            uVar3 = 3;
        } else {
            uVar3 = 1;
        }
    }
    return uVar3;
}
