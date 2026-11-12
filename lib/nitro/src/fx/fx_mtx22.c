// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/fx/src/fx_mtx22.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/fx/fx_mtx22.h>
#include <nitro/fx/fx_const.h>
#include <nitro/fx/fx_cp.h>

inline fx32 mul64 (fx64 x, fx32 y)
{
	return (fx32)((x * y) >> FX32_SHIFT);
}

#include <nitro/code32.h>

#ifndef __MWERKS__
// C for a PC build: MWCC, which defines __MWERKS__, compiles the asm (the SDK's) under #else
void MTX_Identity22_(MtxFx22 *pDst)
{
    pDst->_00 = FX32_ONE;
    pDst->_01 = 0;
    pDst->_10 = 0;
    pDst->_11 = FX32_ONE;
}
#else
asm void MTX_Identity22_ (register MtxFx22 *pDst)
{
	mov r1, #0
	mov r2, #4096
	mov r3, #0
	stmia r0 !, {r2, r3}
	stmia r0 !, {r1, r2}
	bx lr
}
#endif

#include <nitro/code16.h>

#ifndef __MWERKS__
void MTX_Scale22_(MtxFx22 *pDst, fx32 x, fx32 y)
{
    pDst->_00 = x;
    pDst->_01 = 0;
    pDst->_10 = 0;
    pDst->_11 = y;
}
#else
asm void MTX_Scale22_ (register MtxFx22 *pDst, register fx32 x, register fx32 y)
{
	stmia r0 !, {r1}
	mov r1, #0
	str r2, [r0, #8]
	mov r2, #0
	stmia r0 !, {r1, r2}
	bx lr
}
#endif

#ifndef __MWERKS__
void MTX_Rot22_(MtxFx22 *pDst, fx32 sinVal, fx32 cosVal)
{
    pDst->_00 = cosVal;
    pDst->_01 = sinVal;
    pDst->_10 = -sinVal;
    pDst->_11 = cosVal;
}
#else
asm void MTX_Rot22_ (register MtxFx22 *pDst, register fx32 sinVal, register fx32 cosVal)
{
	str r2, [r0, #0]
	str r1, [r0, #4]
	neg r1, r1
	str r1, [r0, #8]
	str r2, [r0, #12]
	bx lr
}
#endif

#include <nitro/codereset.h>

void MTX_ScaleApply22 (const MtxFx22 *pSrc, MtxFx22 *pDst, fx32 x, fx32 y)
{
	fx64 v;

	SDK_NULL_ASSERT(pSrc);
	SDK_NULL_ASSERT(pDst);

	v = (fx64)x;
	pDst->_00 = mul64(v, pSrc->_00);
	pDst->_01 = mul64(v, pSrc->_01);

	v = (fx64)y;
	pDst->_10 = mul64(v, pSrc->_10);
	pDst->_11 = mul64(v, pSrc->_11);
}
