// NitroSystem's NNS_G2dMakeCellToOams, which the Gen 4 games call to turn a cell into OAMs without a renderer (the
// Poketch's sprites), written from its behaviour next to g2d_RendererCore.c's 2D drawing. Not in Black's ROM, so the DS
// build doesn't list this file
#include "nitro/gx.h"
#include "nnsys/g2d.h"

// v * m for a 2x2 matrix, each product rounded
static void MulMtx22_(const MtxFx22 *m, NNSG2dFVec2 *v) {
    NNSG2dFVec2 out;

    out.x = (fx32)((((s64)m->_00 * v->x + 0x800LL) + ((s64)m->_10 * v->y + 0x800LL)) >> FX32_SHIFT);
    out.y = (fx32)((((s64)m->_01 * v->x + 0x800LL) + ((s64)m->_11 * v->y + 0x800LL)) >> FX32_SHIFT);
    *v = out;
}

// Moves an OAM to where the matrix puts it, and gives it the affine parameter. An affine OAM turns about its center:
// the transformed corner lands where the matrix puts the center, less half the OAM's size, and less half again for a
// double-size OAM, which is drawn in a box twice its size
static void SetAffine_(GXOamAttr *oam, const MtxFx22 *mtx, const NNSG2dFVec2 *baseTrans, u16 affineIdx,
                       BOOL doubleAffine) {
    const GXOamShape shape = oam->attr01 & (GX_OAM_ATTR01_SHAPE_MASK | GX_OAM_ATTR01_SIZE_MASK);
    const int halfW = NNS_G2dGetOamSizeX(&shape) >> 1;
    const int halfH = NNS_G2dGetOamSizeY(&shape) >> 1;
    s16 x = (s16)((oam->attr01 & GX_OAM_ATTR01_X_MASK) >> GX_OAM_ATTR01_X_SHIFT);
    s16 y = (s16)((oam->attr01 & GX_OAM_ATTR01_Y_MASK) >> GX_OAM_ATTR01_Y_SHIFT);
    NNSG2dFVec2 pos;

    if (x > 0xff) {
        x |= 0xff00;
    }
    if (y > 0x7f) {
        y |= 0xff00;
    }
    pos.x = x * FX32_ONE;
    pos.y = y * FX32_ONE;
    // A cell's double-size OAM has its corner moved already: back to the plain corner first
    if (G2_GetOBJEffect(oam) == GX_OAM_EFFECT_AFFINE_DOUBLE) {
        pos.x += halfW << FX32_SHIFT;
        pos.y += halfH << FX32_SHIFT;
    }
    MulMtx22_(mtx, &pos);

    G2_SetOBJEffect(oam, doubleAffine ? GX_OAM_EFFECT_AFFINE_DOUBLE : GX_OAM_EFFECT_AFFINE, affineIdx);

    pos.x += -(halfW << FX32_SHIFT) + mtx->_00 * halfW + mtx->_10 * halfH;
    pos.y += -(halfH << FX32_SHIFT) + mtx->_01 * halfW + mtx->_11 * halfH;
    if (doubleAffine) {
        pos.x -= halfW << FX32_SHIFT;
        pos.y -= halfH << FX32_SHIFT;
    }
    pos.x += baseTrans->x;
    pos.y += baseTrans->y;
    G2_SetOBJPosition(oam, pos.x >> FX32_SHIFT, pos.y >> FX32_SHIFT);
}

// Writes the cell's OAMs, as many as fit, moved by baseTrans (and by the matrix, when there is one, with the affine
// parameter affineIdx), and returns how many it wrote
u16 NNS_G2dMakeCellToOams(GXOamAttr *dstOams, u16 numDstOams, const NNSG2dCellData *cell, const MtxFx22 *mtxSR,
                          const NNSG2dFVec2 *baseTrans, u16 affineIdx, BOOL doubleAffine) {
    u16 num = cell->numOAMAttrs;
    u16 i;

    if (num > numDstOams) {
        num = numDstOams;
    }
    for (i = 0; i < num; i++) {
        GXOamAttr *oam = &dstOams[i];

        NNS_G2dCopyCellAsOamAttr(cell, i, oam);
        if (mtxSR != NULL) {
            SetAffine_(oam, mtxSR, baseTrans, affineIdx, doubleAffine);
        } else {
            oam->x += baseTrans->x >> FX32_SHIFT;
            oam->y += baseTrans->y >> FX32_SHIFT;
        }
    }
    return num;
}
