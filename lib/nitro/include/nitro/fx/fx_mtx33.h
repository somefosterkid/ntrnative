// NitroSDK's nitro/fx/fx_mtx33.h, the 3x3 matrices.
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/fx.h, trimmed to what
// Black's C uses so far.
#ifndef BWPORT_NITRO_FX_FX_MTX33_H
#define BWPORT_NITRO_FX_FX_MTX33_H

#include "nitro/fx/fx.h"

void MTX_RotX33_(MtxFx33 *mtx, fx32 sin, fx32 cos);
void MTX_RotY33_(MtxFx33 *mtx, fx32 sin, fx32 cos);
void MTX_RotZ33_(MtxFx33 *mtx, fx32 sin, fx32 cos);
void MTX_Concat33(const MtxFx33 *a, const MtxFx33 *b, MtxFx33 *ab);
void MTX_MultVec33(const VecFx32 *vec, const MtxFx33 *mtx, VecFx32 *dest);
void MTX_Identity33_(MtxFx33 *mtx);

// NitroSDK's MTX_Inverse33
int MTX_Inverse33(const MtxFx33 *mtx, MtxFx33 *inv);

// NitroSDK's MTX_Copy33To44, MTX_Copy43To44 and MTX_Concat44
void MTX_Copy33To44_(const MtxFx33 *src, MtxFx44 *dst);
void MTX_ScaleApply33(const MtxFx33 *src, MtxFx33 *dest, fx32 x, fx32 y, fx32 z);

// NitroSDK's static inline functions that Black's headers lacked, as a final ROM compiles them (no assertions).
// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/fx/fx_mtx33.h
static inline void MTX_Identity33 (MtxFx33 * pDst)
{
	SDK_NULL_ASSERT(pDst);
	MTX_Identity33_(pDst);
}
static inline void MTX_RotX33 (MtxFx33 * pDst, fx32 sinVal, fx32 cosVal)
{
	SDK_NULL_ASSERT(pDst);
	MTX_RotX33_(pDst, sinVal, cosVal);
}
static inline void MTX_RotY33 (MtxFx33 * pDst, fx32 sinVal, fx32 cosVal)
{
	SDK_NULL_ASSERT(pDst);
	MTX_RotY33_(pDst, sinVal, cosVal);
}
static inline void MTX_RotZ33 (MtxFx33 * pDst, fx32 sinVal, fx32 cosVal)
{
	SDK_NULL_ASSERT(pDst);
	MTX_RotZ33_(pDst, sinVal, cosVal);
}

#endif // BWPORT_NITRO_FX_FX_MTX33_H
