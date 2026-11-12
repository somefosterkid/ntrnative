// NitroSDK's nitro/fx/fx_mtx22.h, the 2x2 matrices.
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/fx.h, trimmed to what
// Black's C uses so far.
#ifndef BWPORT_NITRO_FX_FX_MTX22_H
#define BWPORT_NITRO_FX_FX_MTX22_H

#include "nitro/fx/fx.h"

// NitroSDK's MTX_Identity22 and MTX_Scale22
void MTX_Identity22_(MtxFx22 *mtx);
void MTX_Scale22_(MtxFx22 *mtx, fx32 x, fx32 y);

// Rotations about an axis from its sine and cosine: NitroSDK's MTX_Rot22_, MTX_RotX33_ and the rest
void MTX_Rot22_(MtxFx22 *mtx, fx32 sin, fx32 cos);

// NitroSDK's MTX_ScaleApply22 and MTX_Concat33
void MTX_ScaleApply22(const MtxFx22 *src, MtxFx22 *dest, fx32 x, fx32 y);

// NitroSDK's static inline functions that Black's headers lacked, as a final ROM compiles them (no assertions).
// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/fx/fx_mtx22.h
static inline void MTX_Rot22 (MtxFx22 * pDst, fx32 sinVal, fx32 cosVal)
{
	SDK_NULL_ASSERT(pDst);
	MTX_Rot22_(pDst, sinVal, cosVal);
}

#endif // BWPORT_NITRO_FX_FX_MTX22_H
