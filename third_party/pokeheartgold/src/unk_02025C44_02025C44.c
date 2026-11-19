#include "global.h"

#include "unk_02025C44.h"


void sub_02025C54(NNSG2dRenderSurface *surface, NNSG2dViewRect *viewRect, NNSG2dOamRegisterFunction oamRegisterFunc, NNSG2dAffineRegisterFunction oamAffineRegisterFunc, NNSG2dRndCellCullingFunction visibilityCullingFunc, NNSG2dSurfaceType surfaceType, NNSG2dRendererInstance *renderer);

void GF_InitG2dRenderer(NNSG2dRendererInstance *instance, s32 zOffset) {
    NNS_G2dInitRenderer(instance);
    instance->spriteZoffsetStep = zOffset;
}

void sub_02025C54(NNSG2dRenderSurface *surface, NNSG2dViewRect *viewRect, NNSG2dOamRegisterFunction oamRegisterFunc, NNSG2dAffineRegisterFunction oamAffineRegisterFunc, NNSG2dRndCellCullingFunction visibilityCullingFunc, NNSG2dSurfaceType surfaceType, NNSG2dRendererInstance *renderer) {
    NNS_G2dInitRenderSurface(surface);

    surface->viewRect = *viewRect;
    surface->pFuncOamRegister = oamRegisterFunc;
    surface->pFuncOamAffineRegister = oamAffineRegisterFunc;
    surface->pFuncVisibilityCulling = visibilityCullingFunc;
    surface->type = surfaceType;

    if (renderer) {
        NNS_G2dAddRendererTargetSurface(renderer, surface);
    }
}
