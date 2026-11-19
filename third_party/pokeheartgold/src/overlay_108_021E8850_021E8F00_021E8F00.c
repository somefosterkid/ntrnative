#include "global.h"

#include "constants/heap.h"

#include "camera.h"
#include "filesystem.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "overlay_108_021E8850.h"
#include "overlay_108_safari_decoration_app.h"

extern s16 ov108_021EA9E4[];

void ov108_021E8F00(SafariDecoration3dView *view, s32 id, const SafariObjectConfig *config) {
    SafariDecoration3dModel *model;
    NNSG3dResMdlSet *mdlSet;
    NNSG3dResTex *tex;
    s16 *pos;
    s32 *entry;
    s32 i;
    s32 buf[6];

    model = &view->model;
    model->file = GfGfxLoader_LoadFromOpenNarc(view->narcModels, config->buildModel, FALSE, view->heapID, FALSE);
    mdlSet = NNS_G3dGetMdlSet(model->file);
    model->mdlSet = mdlSet;
    model->mdl = NNS_G3dGetMdlByIdx(mdlSet, 0);
    tex = NNS_G3dGetTex(model->file);
    model->tex = tex;
    if (tex != NULL) {
        GF3dRender_AllocAndLoadTexResources(tex);
        GF3dRender_BindModelSet(model->file, model->tex);
    }
    NNS_G3dRenderObjInit(&model->renderObj, model->mdl);
    DC_FlushAll();
    model->loaded = 1;
    NARC_ReadWholeMember(view->narcAnimTable, config->buildModel, buf);
    if (((u8 *)buf)[0] != 0 && ((u8 *)buf)[1] != 8) {
        i = 0;
        entry = buf;
        for (; i < 4; i++) {
            if (entry[2] == -1) {
                break;
            }
            ov108_021E9198(view, model, &model->anims[model->numAnims], entry[2]);
            ov108_021E91F8(model, &model->anims[model->numAnims]);
            entry++;
            model->numAnims++;
        }
    }
    pos = &ov108_021EA9E4[id * 4];
    view->target.x = FX32_CONST(pos[1]);
    view->target.y = FX32_CONST(pos[2]);
    view->target.z = FX32_CONST(pos[3]);
    Camera_SetDistance(FX32_CONST((u16)pos[0]), view->camera);
}
