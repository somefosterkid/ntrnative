#include "global.h"

void ov51_021E6C00(void);

void ov51_021E6C00(void) {
    MtxFx22 affineTransform;

    fx32 xScale = FX_Inv(1 << 12);
    fx32 yScale = FX_Inv(1 << 12);
    affineTransform._00 = xScale;
    affineTransform._01 = 0;
    affineTransform._10 = 0;
    affineTransform._11 = yScale;

    OS_WaitVBlankIntr();

    G2S_SetBG2Affine(&affineTransform, 256 / 2, 192 / 2, 0, 0);
    G2S_SetBG3Affine(&affineTransform, 256 / 2, 192 / 2, 0, 0);

    OS_WaitVBlankIntr();

    G2_SetBG3Affine(&affineTransform, 256 / 2, 0, 0, 0);
}
