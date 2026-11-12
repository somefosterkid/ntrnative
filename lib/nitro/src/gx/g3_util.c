// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/gx/src/g3_util.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/gx/g3_util.h>
#include <nitro/gx/g3imm.h>
#include <nitro/gx/g3b.h>
#include <nitro/gx/g3c.h>
#include <nitro/gx/gx_vramcnt.h>
#include <nitro/fx/fx_cp.h>
#include <nitro/fx/fx_vec.h>
#include <nitro/hw/ARM9/mmap_global.h>
#include <nitro/mi/dma.h>
#include <nitro/mi/memory.h>

void G3i_PerspectiveW_ (fx32 fovySin, fx32 fovyCos, fx32 aspect, fx32 n, fx32 f, fx32 scaleW, BOOL draw, MtxFx44 *mtx)
{
	MtxFx44 tmp[1];

	if (mtx == NULL) {
		mtx = tmp;
	}

	MTX_PerspectiveW(fovySin, fovyCos, aspect, n, f, scaleW, mtx);

	if (draw) {
		G3_MtxMode(GX_MTXMODE_PROJECTION);
		G3_LoadMtx44(mtx);
	}
}

void G3i_OrthoW_ (fx32 t, fx32 b, fx32 l, fx32 r, fx32 n, fx32 f, fx32 scaleW, BOOL draw, MtxFx44 *mtx)
{
	MtxFx44 tmp[1];

	if (mtx == NULL) {
		mtx = tmp;
	}

	MTX_OrthoW(t, b, l, r, n, f, scaleW, mtx);

	if (draw) {
		G3_MtxMode(GX_MTXMODE_PROJECTION);
		G3_LoadMtx44(mtx);
	}
}

void G3i_LookAt_ (const VecFx32 *camPos, const VecFx32 *camUp, const VecFx32 *target, BOOL draw, MtxFx43 *mtx)
{
	MtxFx43 tmp[1];

	if (mtx == NULL) {
		mtx = tmp;
	}

	MTX_LookAt(camPos, camUp, target, mtx);

	if (draw) {
		G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
		G3_LoadMtx43(mtx);
	}
}

void G3_RotX (fx32 s, fx32 c)
{
	vs32 *p = (vs32 *)&reg_G3_MTX_MULT_3x3;

	SDK_MINMAX_ASSERT(s, -FX32_ONE, FX32_ONE);
	SDK_MINMAX_ASSERT(c, -FX32_ONE, FX32_ONE);

	*p = FX32_ONE;
	*p = 0;
	*p = 0;
	*p = 0;
	*p = c;
	*p = s;
	*p = 0;
	*p = -s;
	*p = c;
}

void G3_RotY (fx32 s, fx32 c)
{
	vs32 *p = (vs32 *)&reg_G3_MTX_MULT_3x3;

	SDK_MINMAX_ASSERT(s, -FX32_ONE, FX32_ONE);
	SDK_MINMAX_ASSERT(c, -FX32_ONE, FX32_ONE);

	*p = c;
	*p = 0;
	*p = -s;
	*p = 0;
	*p = FX32_ONE;
	*p = 0;
	*p = s;
	*p = 0;
	*p = c;
}

void G3_RotZ (fx32 s, fx32 c)
{
	vs32 *p = (vs32 *)&reg_G3_MTX_MULT_3x3;

	SDK_MINMAX_ASSERT(s, -FX32_ONE, FX32_ONE);
	SDK_MINMAX_ASSERT(c, -FX32_ONE, FX32_ONE);

	*p = c;
	*p = s;
	*p = 0;
	*p = -s;
	*p = c;
	*p = 0;
	*p = 0;
	*p = 0;
	*p = FX32_ONE;
}

#include <nitro/code32.h>

#include <nitro/codereset.h>

#include <nitro/code32.h>

#include <nitro/codereset.h>
