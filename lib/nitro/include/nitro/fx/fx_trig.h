// NitroSDK's nitro/fx/fx_trig.h, the sines, cosines and their inverses, by 16-bit angle.
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/fx.h, trimmed to what
// Black's C uses so far.
#ifndef BWPORT_NITRO_FX_FX_TRIG_H
#define BWPORT_NITRO_FX_FX_TRIG_H

#include "nitro/fx/fx.h"

// The sine and cosine of 4096 angles around the circle, as pairs
extern const fx16 FX_SinCosTable_[4096 * 2];

// An angle in whole degrees as the 16-bit angle that FX_SinIdx and FX_CosIdx take, 0x10000 for a full turn
#define DEG_TO_IDX(deg) ((u16)((deg) * 0x10000 / 360))
static inline fx16 FX_SinIdx(int idx) {
    return FX_SinCosTable_[(idx >> 4) << 1];
}
static inline fx16 FX_CosIdx(int idx) {
    return FX_SinCosTable_[((idx >> 4) << 1) + 1];
}

// NitroSDK's FX_Atan2Idx
u16 FX_Atan2Idx(fx32 y, fx32 x);
#define FX_DEG_TO_IDX(deg) ((u16)(((deg) * FX64C_65536_360 + 0x80000000000LL) >> 44))

// NitroSDK's FX_AcosIdx
u16 FX_AcosIdx(fx32 x);

#endif // BWPORT_NITRO_FX_FX_TRIG_H
