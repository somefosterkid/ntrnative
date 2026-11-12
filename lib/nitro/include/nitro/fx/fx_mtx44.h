// NitroSDK's nitro/fx/fx_mtx44.h, the 4x4 matrices.
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/fx.h, trimmed to what
// Black's C uses so far.
#ifndef BWPORT_NITRO_FX_FX_MTX44_H
#define BWPORT_NITRO_FX_FX_MTX44_H

#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"

// Projection and camera matrices: NitroSDK's MTX_PerspectiveW and MTX_LookAt
void MTX_PerspectiveW(fx32 fovySin, fx32 fovyCos, fx32 aspect, fx32 n, fx32 f, fx32 scaleW, MtxFx44 *mtx);
void MTX_OrthoW(fx32 t, fx32 b, fx32 l, fx32 r, fx32 n, fx32 f, fx32 scaleW, MtxFx44 *mtx);
void MTX_Concat44(const MtxFx44 *a, const MtxFx44 *b, MtxFx44 *ab);
void MTX_FrustumW(fx32 t, fx32 b, fx32 l, fx32 r, fx32 n, fx32 f, fx32 scaleW, MtxFx44 *mtx);

// fx_mtx44.c. Swan: MAT4_Identity, MAT4_To4x3, MTX_Concat44, MTX_FrustumW, MTX_PerspectiveW, MTX_OrthoW
void MTX_Identity44_(MtxFx44 *mtx);
void MTX_Copy44To43_(const MtxFx44 *src, MtxFx43 *dst);

// NitroSDK's static inline functions that Black's headers lacked, as a final ROM compiles them (no assertions).
// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/fx/fx_mtx44.h
static inline void MTX_Identity44 (MtxFx44 * pDst)
{
	SDK_NULL_ASSERT(pDst);
	MTX_Identity44_(pDst);
}
static inline void MTX_Copy44To43 (const MtxFx44 * pSrc, MtxFx43 * pDst)
{
	SDK_NULL_ASSERT(pSrc);
	SDK_NULL_ASSERT(pDst);
	MTX_Copy44To43_(pSrc, pDst);
}
static inline void MTX_Frustum (fx32 t, fx32 b, fx32 l, fx32 r, fx32 n, fx32 f, MtxFx44 * mtx)
{
	MTX_FrustumW(t, b, l, r, n, f, FX32_ONE, mtx);
}
static inline
void MTX_Perspective (fx32 fovySin, fx32 fovyCos, fx32 aspect, fx32 n, fx32 f, MtxFx44 * mtx)
{
	MTX_PerspectiveW(fovySin, fovyCos, aspect, n, f, FX32_ONE, mtx);
}
static inline void MTX_Ortho (fx32 t, fx32 b, fx32 l, fx32 r, fx32 n, fx32 f, MtxFx44 * mtx)
{
	MTX_OrthoW(t, b, l, r, n, f, FX32_ONE, mtx);
}

#endif // BWPORT_NITRO_FX_FX_MTX44_H
