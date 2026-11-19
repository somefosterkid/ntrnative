#include "global.h"

void ov96_0220404C(s32, s32, u32 *, u32 *);

extern void *ov96_0221C98C;

u8 ov96_022031A8(void *param_1, u8 param_2, s32 param_3, s32 param_4, void *param_5);

u8 ov96_022031A8(void *param_1, u8 param_2, s32 param_3, s32 param_4, void *param_5) {
    s32 iVar1;
    s32 iVar2;
    fx32 fVar3;
    u32 uVar4;
    s32 iVar5;
    VecFx32 *a;
    VecFx32 VStack_24;
    MtxFx43 MStack_54;
    VecFx32 VStack_60;
    VecFx32 VStack_6c;
    VecFx32 VStack_78;
    u32 uStack_80;
    u32 uStack_7c;

    uVar4 = (u32)param_2;
    ov96_0220404C(param_3, param_4 + 0x120, &uStack_80, &uStack_7c);
    VStack_24.x = (s32)(*(f32 *)&uStack_80 * 4096.0f);
    VStack_24.z = (s32)(*(f32 *)&uStack_7c * 4096.0f);
    VStack_24.y = 0;
    if (uVar4 != 0) {
        VStack_60 = VStack_24;
        VStack_60.x -= 0x21000;
        VStack_60.z -= 0x21000;
        iVar2 = uVar4 * 0x4000;

        MTX_RotY43_(&MStack_54, FX_SinIdx(iVar2), FX_CosIdx(iVar2));
        MTX_MultVec43(&VStack_60, &MStack_54, &VStack_6c);
        VStack_6c.x += 0x21000;
        VStack_6c.z += 0x21000;
        VStack_24 = VStack_6c;
    }
    *(VecFx32 *)param_5 = VStack_24;
    iVar5 = uVar4 * 3;
    iVar1 = (uVar4 + 1) * 3;
    if (iVar5 < iVar1) {
        a = (VecFx32 *)&ov96_0221C98C + iVar5;
        do {
            VEC_Subtract(a, &VStack_24, &VStack_78);
            fVar3 = VEC_Mag(&VStack_78);
            if (fVar3 <= 0x4000) {
                return (u8)iVar5;
            }
            iVar5 = iVar5 + 1;
            a = a + 1;
        } while (iVar5 < iVar1);
    }
    return 0xc;
}
