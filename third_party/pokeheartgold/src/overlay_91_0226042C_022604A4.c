#include "global.h"

extern const u16 ov91_02261D3C[4][4];

void ov91_022604A4(const VecFx32 *param0, VecFx32 *param1, u32 param2, u32 param3);

void ov91_022604A4(const VecFx32 *param0, VecFx32 *param1, u32 param2, u32 param3) {
    MtxFx33 v0;

    MTX_Identity33(&v0);
    MTX_RotY33(&v0, FX_SinIdx(ov91_02261D3C[param3][param2]), FX_CosIdx(ov91_02261D3C[param3][param2]));
    MTX_Inverse33(&v0, &v0);
    MTX_MultVec33(param0, &v0, param1);
}
