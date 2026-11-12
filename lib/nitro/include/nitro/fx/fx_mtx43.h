// NitroSDK's nitro/fx/fx_mtx43.h, the 4x3 matrices.
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/fx.h, trimmed to what
// Black's C uses so far.
#ifndef BWPORT_NITRO_FX_FX_MTX43_H
#define BWPORT_NITRO_FX_FX_MTX43_H

#include "nitro/fx/fx.h"

void MTX_LookAt(const VecFx32 *camPos, const VecFx32 *camUp, const VecFx32 *target, MtxFx43 *mtx);

// NitroSDK's MTX_Inverse43: returns -1 when the matrix has no inverse
int MTX_Inverse43(const MtxFx43 *mtx, MtxFx43 *inv);
void MTX_MultVec43(const VecFx32 *vec, const MtxFx43 *mtx, VecFx32 *dest);

// NitroSDK's MTX_Identity43, MTX_Scale43 and MTX_Concat43
void MTX_Identity43_(MtxFx43 *mtx);
void MTX_Scale43_(MtxFx43 *mtx, fx32 x, fx32 y, fx32 z);
void MTX_Concat43(const MtxFx43 *a, const MtxFx43 *b, MtxFx43 *ab);
void MTX_Copy43To44_(const MtxFx43 *src, MtxFx44 *dst);
void MTX_RotAxis43(MtxFx43 *mtx, const VecFx32 *axis, fx32 sinVal, fx32 cosVal);
void MTX_ScaleApply43(const MtxFx43 *src, MtxFx43 *dest, fx32 x, fx32 y, fx32 z);
void MTX_RotY43_(MtxFx43 *mtx, fx32 sin, fx32 cos);

// NitroSDK's static inline functions that Black's headers lacked, as a final ROM compiles them (no assertions).
// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/fx/fx_mtx43.h
static inline void MTX_RotY43 (MtxFx43 * pDst, fx32 sinVal, fx32 cosVal)
{
	SDK_NULL_ASSERT(pDst);
	MTX_RotY43_(pDst, sinVal, cosVal);
}

#endif // BWPORT_NITRO_FX_FX_MTX43_H
