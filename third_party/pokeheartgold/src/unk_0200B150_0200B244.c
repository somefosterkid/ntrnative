#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "unk_0200B150.h"
#include "oam_manager.h"


void OamManager_Free(void) {
    if (_021D0EB0 == NULL) {
        GF_AssertFail();
    }
    thunk_ClearMainOAM(_021D0EB0->heapID);
    thunk_ClearSubOAM(_021D0EB0->heapID);
    Heap_Free(_021D0EB0);
    _021D0EB0 = NULL;
}

void sub_02025C54(NNSG2dRenderSurface *surface, NNSG2dViewRect *viewRect, NNSG2dOamRegisterFunction oamRegisterFunc, NNSG2dAffineRegisterFunction oamAffineRegisterFunc, NNSG2dRndCellCullingFunction visibilityCullingFunc, enum NNS_G2D_VRAM_TYPE surfaceType, NNSG2dRendererInstance *renderer);

BOOL sub_0200B2F0(const GXOamAttr *pOam, u16 affineIdx, BOOL doubleAffine);
BOOL sub_0200B310(const GXOamAttr *pOam, u16 affineIdx, BOOL doubleAffine);
u16 sub_0200B334(const MtxFx22 *mtx);
u16 sub_0200B358(const MtxFx22 *mtx);
BOOL sub_02025C98(const NNSG2dCellData *cellData, const MtxFx32 *objectCoords, const NNSG2dViewRect *viewRect);

void sub_0200B27C(NNSG2dRenderSurface *surface, NNSG2dViewRect *rect, enum NNS_G2D_VRAM_TYPE type, NNSG2dRendererInstance *instance) {
    if (_021D0EB0 == 0) {
        GF_AssertFail();
    }
    if (type == NNS_G2D_VRAM_TYPE_2DMAIN) {
        sub_02025C54(surface, rect, sub_0200B2F0, sub_0200B334, sub_02025C98, type, instance);
        return;
    }
    sub_02025C54(surface, rect, sub_0200B310, sub_0200B358, sub_02025C98, type, instance);
}
