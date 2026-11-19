#include "global.h"

void ov51_021E69EC(u8 *param_1);

void ov51_021E69EC(u8 *param_1) {
    MtxFx22 MStack_18;
    char new_var;
    MStack_18._00 = FX_Inv(*((fx32 *)(param_1 + 0x30fc)));
    MStack_18._01 = 0;
    MStack_18._10 = 0;
    MStack_18._11 = FX_Inv(*((fx32 *)(param_1 + 0x3100)));
    OS_WaitVBlankIntr();
    G2S_SetBG2Affine(&MStack_18, 0x80, 0x60, 0, 0);
    G2S_SetBG3Affine(&MStack_18, 0x80, 0x60, 0, 0);
    *((u8 *)(param_1 + 0x343a)) = (*((u8 *)(param_1 + 0x343a))) & (new_var = 0xfb);
    return;
}
