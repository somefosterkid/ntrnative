#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "global.h"

#include "error_handling.h"
#include "gf_3d_loader.h"
#include "overlay_01_021F1348.h"

void ov01_021F1AB8(FieldEffectManager *fieldEffMan, u32 fileId, s32 allocAtEnd, NNSG3dRenderObj *renderObj, NNSG3dResMdl **model, NNSG3dResFileHeader **resFile) {
    *resFile = ov01_021F14B4(fieldEffMan, fileId, allocAtEnd);
    ov01_021F1A48(fieldEffMan, renderObj, model, resFile);
}

NNSG3dResTex *ov01_021F1AD4(FieldEffectManager *fieldEffMan, s32 arg1);

NNSG3dResTex *ov01_021F1AD4(FieldEffectManager *fieldEffMan, s32 id) {
    NNSG3dResTex *nNSG3dResTex;
    GF_3DGfxRawResObj *obj;

    obj = GF3dGfxRawResMan_GetObjById(fieldEffMan->unk_20->texResMan, id);
    nNSG3dResTex = NULL;
    if (obj == NULL) {
        GF_AssertFail();
    }
    if (obj != NULL) {
        nNSG3dResTex = GF3dGfxRawResObj_GetTex(obj);
    }
    return nNSG3dResTex;
}
