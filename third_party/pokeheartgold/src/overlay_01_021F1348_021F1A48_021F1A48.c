#include "global.h"

#include "error_handling.h"
#include "gf_3d_render.h"
#include "overlay_01_021F1348.h"

void ov01_021F1A48(FieldEffectManager *fieldEffMan, NNSG3dRenderObj *renderObj, NNSG3dResMdl **model, NNSG3dResFileHeader **resFile);

void ov01_021F1A48(FieldEffectManager *fieldEffMan, NNSG3dRenderObj *renderObj, NNSG3dResMdl **model, NNSG3dResFileHeader **resFile) {
    NNSG3dResTex *tex;
    BOOL BVar1;
    NNSG3dResDictTreeNode *new_var;
    NNSG3dResMdlSet *pNVar2;
    NNSG3dResMdl *iVar3;
    s32 *piVar4;
    u32 idx = 0;
    tex = NNS_G3dGetTex(*resFile);
    if ((tex != 0L) && (BVar1 = GF3dRender_ResTexIsLoaded(tex), 0 == BVar1)) {
        DC_FlushRange(*resFile, (*resFile)->fileSize);
        BVar1 = NNS_G3dResDefaultSetup(*resFile);
        if (BVar1 == 0) {
            GF_AssertFail();
        }
    }
    pNVar2 = NNS_G3dGetMdlSet(*resFile);
    if (pNVar2 != 0L) {
        new_var = pNVar2->dict.node + (-1);
        if ((!((&pNVar2->dict) == 0L)) && (pNVar2->dict.numEntry > idx)) {
            piVar4 = (s32 *)(((u8 *)(&pNVar2->dict)) + ((u32)pNVar2->dict.ofsEntry) + 4);
        } else {
            piVar4 = 0L;
        }
        if (piVar4 != 0L) {
            iVar3 = (NNSG3dResMdl *)(((u8 *)pNVar2) + (*piVar4));
            goto code_r0x0200005c;
        }
    }
    iVar3 = 0;
code_r0x0200005c:
    *model = iVar3;

    if (model == 0L) {
        GF_AssertFail();
    }
    NNS_G3dRenderObjInit(renderObj, *model);
    return;
}
