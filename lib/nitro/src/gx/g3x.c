// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/gx/src/g3x.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/gx/g3x.h>
#include <nitro/gx/g3imm.h>
#include <nitro/gx/gx.h>
#include <nitro/gx/gx_bgcnt.h>
#include <nitro/hw/ARM9/ioreg_GX.h>
#include <nitro/hw/ARM9/ioreg_G3.h>
#include <nitro/hw/ARM9/ioreg_G2.h>
#include <nitro/fx/fx_const.h>
#include <nitro/mi/dma.h>
#include <nitro/mi/memory.h>

#include "gxasm.h"

#ifndef __MWERKS__
static void GXi_NopClearFifo128_(void *pDest);
#else
static asm void GXi_NopClearFifo128_(register void *pDest);
#endif

void MI_NDmaFillAsync(u32 dmaNo, void *dest, u32 data, u32 size, MIDmaCallback callback, void *arg);

void G3X_Init (void)
{

	G3X_ClearFifo();
	G3_End();

	while ( G3X_IsGeometryBusy()) {
	}

	reg_G3X_DISP3DCNT = 0;
	reg_G3X_GXSTAT = 0;
	reg_G2_BG0OFS = 0;

	G3X_ResetListRamOverflow();
	G3X_ResetLineBufferUnderflow();
	G3X_SetShading(GX_SHADING_TOON);
	G3X_AntiAlias(TRUE);
	G3X_AlphaTest(FALSE, 0);

	G3X_ResetMtxStackOverflow();
	G3X_SetFifoIntrCond(GX_FIFOINTR_COND_EMPTY);
	G3X_InitMtxStack();

	reg_G3X_CLEAR_COLOR = 0;
	reg_G3X_CLEAR_DEPTH = 0x7fff;
	reg_G3X_CLRIMAGE_OFFSET = 0;
	reg_G3X_FOG_COLOR = 0;
	reg_G3X_FOG_OFFSET = 0;

	G2_SetBG0Priority(0);
	G3X_InitTable();
	G3_PolygonAttr(GX_LIGHTMASK_NONE, GX_POLYGONMODE_MODULATE, GX_CULL_BACK, 0, 31, GX_POLYGON_ATTR_MISC_NONE);

	G3_TexImageParam(GX_TEXFMT_NONE,
	                 GX_TEXGEN_NONE,
	                 GX_TEXSIZE_S8, GX_TEXSIZE_T8, GX_TEXREPEAT_NONE, GX_TEXFLIP_NONE, GX_TEXPLTTCOLOR0_USE, 0);

	G3_Direct1(G3OP_TEXPLTT_BASE, 0);
}

void G3X_Reset (void)
{

	while (G3X_IsGeometryBusy()) {
	}

	G3X_ResetMtxStackOverflow();
	G3X_ResetListRamOverflow();
	G3X_ResetLineBufferUnderflow();
	G3X_ResetMtxStack();

	G3_PolygonAttr(GX_LIGHTMASK_NONE, GX_POLYGONMODE_MODULATE, GX_CULL_BACK, 0, 31,
	               GX_POLYGON_ATTR_MISC_NONE);

	G3_TexImageParam(GX_TEXFMT_NONE,
	                 GX_TEXGEN_NONE,
	                 GX_TEXSIZE_S8, GX_TEXSIZE_T8, GX_TEXREPEAT_NONE, GX_TEXFLIP_NONE,
	                 GX_TEXPLTTCOLOR0_USE, 0);

	G3_Direct1(G3OP_TEXPLTT_BASE, 0);
}

void G3X_ClearFifo (void)
{
	GXi_NopClearFifo128_((void *)&reg_G3X_GXFIFO);

	while (G3X_IsGeometryBusy()) {
	}
}

void G3X_InitMtxStack (void)
{
	s32 levelPV;
	s32 levelPJ;
	G3X_ResetMtxStackOverflow();

	while (G3X_GetMtxStackLevelPV(&levelPV))
		;

	while (G3X_GetMtxStackLevelPJ(&levelPJ))
		;

	G3_MtxMode(GX_MTXMODE_TEXTURE);
	G3_Identity();
	G3_MtxMode(GX_MTXMODE_PROJECTION);

	if (levelPJ != 0) {
		G3_PopMtx(levelPJ);
	}

	G3_Identity();
	G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
	G3_PopMtx(levelPV);
	G3_Identity();
}

void G3X_ResetMtxStack (void)
{
	s32 levelPV;
	s32 levelPJ;
	G3X_ResetMtxStackOverflow();

	while (G3X_GetMtxStackLevelPV(&levelPV))
		;

	while (G3X_GetMtxStackLevelPJ(&levelPJ))
		;

	G3_MtxMode(GX_MTXMODE_TEXTURE);
	G3_Identity();
	G3_MtxMode(GX_MTXMODE_PROJECTION);

	if (levelPJ != 0) {
		G3_PopMtx(levelPJ);
	}

	G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
	G3_PopMtx(levelPV);
	G3_Identity();

}

void G3X_SetFog (BOOL enable, GXFogBlend fogMode, GXFogSlope fogSlope, int fogOffset)
{
	if (enable) {
		SDK_MINMAX_ASSERT(fogOffset, 0, 0x7fff);
		SDK_MINMAX_ASSERT(fogSlope, GX_FOGSLOPE_0x8000, GX_FOGSLOPE_0x0020);
		SDK_ASSERT(GX_FOGBLEND_COLOR_ALPHA == fogMode || GX_FOGBLEND_ALPHA == fogMode);

		reg_G3X_FOG_OFFSET = (u16)fogOffset;

		reg_G3X_DISP3DCNT = (u16)((reg_G3X_DISP3DCNT &
		                           ~(REG_G3X_DISP3DCNT_FOG_SHIFT_MASK
		                             | REG_G3X_DISP3DCNT_FMOD_MASK
		                             | REG_G3X_DISP3DCNT_RO_MASK
		                             | REG_G3X_DISP3DCNT_GO_MASK))
		                          | ((fogSlope << REG_G3X_DISP3DCNT_FOG_SHIFT_SHIFT)
		                             | (fogMode << REG_G3X_DISP3DCNT_FMOD_SHIFT)
		                             | (REG_G3X_DISP3DCNT_FME_MASK)));
	} else {
		reg_G3X_DISP3DCNT &= (u16) ~(REG_G3X_DISP3DCNT_FME_MASK
		                             | REG_G3X_DISP3DCNT_RO_MASK | REG_G3X_DISP3DCNT_GO_MASK);
	}
}

int G3X_GetClipMtx (MtxFx44 *m)
{
	SDK_NULL_ASSERT(m);

	if (G3X_IsGeometryBusy()) {
		return -1;
	} else {
		MI_Copy64B((void *)REG_CLIPMTX_RESULT_0_ADDR, &m->_00);
		return 0;
	}
}

int G3X_GetVectorMtx (MtxFx33 *m)
{
	SDK_NULL_ASSERT(m);

	if (G3X_IsGeometryBusy()) {
		return -1;
	} else {
		MI_Copy36B((void *)(REG_VECMTX_RESULT_0_ADDR), &m->_00);
		return 0;
	}
}

void G3X_SetEdgeColorTable (const GXRgb *rgb_8)
{
	SDK_NULL_ASSERT(rgb_8);
	MI_CpuCopy16(rgb_8, (void *)REG_EDGE_COLOR_0_L_ADDR, 16);
}

void G3X_SetFogTable (const u32 *fogTable)
{
	SDK_NULL_ASSERT(fogTable);
	MI_Copy32B(&fogTable[0], (void *)REG_FOG_TABLE_0_ADDR);
}

void G3X_SetClearColor (GXRgb rgb, int alpha, int depth, int polygonID, BOOL fog)
{
	u32 val;
	GX_POLYGONID_ASSERT(polygonID);
	GX_ALPHA_ASSERT(alpha);
	GXRGB_ASSERT(rgb);
	GX_DEPTH_ASSERT(depth);

	val = (u32)(rgb | (alpha << REG_G3X_CLEAR_COLOR_ALPHA_SHIFT) | (polygonID << REG_G3X_CLEAR_COLOR_POLYGONID_SHIFT));

	if (fog)
		val |= REG_G3X_CLEAR_COLOR_F_MASK;

	reg_G3X_CLEAR_COLOR = val;
	reg_G3X_CLEAR_DEPTH = (u16)depth;
}

void G3X_InitTable (void)
{
	int i;

	if (GXi_DmaId != GX_DMA_NOT_USE) {
		if (GXi_DmaId > 3) {
			MI_NDmaFillAsync(GXi_DmaId - 4, (void *)REG_EDGE_COLOR_0_L_ADDR, 0, 16, NULL, NULL);
			MI_NDmaFill(GXi_DmaId - 4, (void *)REG_FOG_TABLE_0_ADDR, 0, 96);
		} else {
			MIi_DmaFill32Async(GXi_DmaId, (void *)REG_EDGE_COLOR_0_L_ADDR, 0, 16, NULL, NULL, TRUE);
			MIi_DmaFill32(GXi_DmaId, (void *)REG_FOG_TABLE_0_ADDR, 0, 96, TRUE);
		}
	} else {
		MI_CpuClear32((void *)REG_EDGE_COLOR_0_L_ADDR, 16);
		MI_CpuClear32((void *)REG_FOG_TABLE_0_ADDR, 96);
	}

	for (i = 0; i < 32; i++) {
		reg_G3_SHININESS = 0;
	}
}

int G3X_GetMtxStackLevelPV (s32 *level)
{
	SDK_NULL_ASSERT(level);

	if (reg_G3X_GXSTAT & REG_G3X_GXSTAT_SB_MASK) {
		return -1;
	} else {
		*level = (s32)((reg_G3X_GXSTAT & REG_G3X_GXSTAT_PV_MASK) >> REG_G3X_GXSTAT_PV_SHIFT);
		return 0;
	}
}

int G3X_GetMtxStackLevelPJ (s32 *level)
{
	SDK_NULL_ASSERT(level);

	if (reg_G3X_GXSTAT & REG_G3X_GXSTAT_SB_MASK) {
		return -1;
	} else {
		*level = (s32)((reg_G3X_GXSTAT & REG_G3X_GXSTAT_PJ_MASK) >> REG_G3X_GXSTAT_PJ_SHIFT);
		return 0;
	}
}

void G3X_SetDisp1DotDepth (fx32 w)
{
	SDK_MINMAX_ASSERT(w, 0, FX32_ONE * 0x1000 - 1);
	reg_G3X_DISP_1DOT_DEPTH = (u16)((w >> 9) & 0x7fff);
}

int G3X_GetBoxTestResult (s32 *in)
{
	SDK_NULL_ASSERT(in);

	if (reg_G3X_GXSTAT & REG_G3X_GXSTAT_TB_MASK)
		return -1;
	else {
		*in = (s32)(reg_G3X_GXSTAT & REG_G3X_GXSTAT_TR_MASK);
		return 0;
	}
}

#include <nitro/code32.h>

#ifndef __MWERKS__
// C for a PC build (MWCC, which defines __MWERKS__, compiles the asm under #else): 128 words of zero, each four to
// pDest's first four words (the geometry FIFO's port)
static void GXi_NopClearFifo128_(void *pDest)
{
    vu32 *d = (vu32 *)pDest;
    int i;

    for (i = 0; i < 32; i++) {
        d[0] = 0;
        d[1] = 0;
        d[2] = 0;
        d[3] = 0;
    }
}
#else
static asm void GXi_NopClearFifo128_ (register void *pDest)
{
	mov r1, #0
	mov r2, #0
	mov r3, #0
	mov r12, #0
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	stmia r0, {r1 - r3, r12}
	bx lr
}
#endif

#include <nitro/codereset.h>
