#include "global.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"

NNSG3dResMdlSet *sub_02024328(const BillboardResources *resources, NNSG3dResMdl **outModel, NNSG3dResTex **outTexture);

NNSG3dResTex *sub_02024374(const BillboardResources *resources);

void sub_02023CF0(Billboard *billboard, const BillboardResources *resources);

Billboard *sub_02024280(BillboardList *list);
void sub_02024380(Billboard *billboard, const BillboardResources *resources);
void sub_02023D24(Billboard *billboard, const BillboardResources *resources);
Billboard *sub_02023D44(const BillboardTemplate *template);

void sub_02023CF0(Billboard *billboard, const BillboardResources *resources) {
    billboard->modelSet = sub_02024328(resources, &billboard->model, &billboard->texture);
    NNS_G3dRenderObjInit(&billboard->renderObj, billboard->model);
    billboard->animTexture = sub_02024374(resources);
}

void sub_02023D24(Billboard *billboard, const BillboardResources *resources) {
    billboard->gfxSequence = resources->gfxSequence;
    billboard->vramTransfer = 0L;
}

Billboard *sub_02023D44(const BillboardTemplate *template) {
    if (template->list == 0L) {
        return 0L;
    }

    BillboardList *list = template->list;
    Billboard *billboard = sub_02024280(list);

    if (billboard == 0L) {
        return 0L;
    }

    billboard->list = list;
    billboard->pos = template->pos;
    billboard->scale = template->scale;
    billboard->animNum = 0;
    billboard->draw = 1;
    billboard->state = BILLBOARD_STATE_INITIALIZED;

    sub_02024380(billboard, template->resources);

    return billboard;
}
