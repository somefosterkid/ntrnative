#include "global.h"

#include "overlay_01_021F1348.h"
#include "overlay_01_021FE200.h"
#include "unk_020689C8.h"

void sub_020699BC(Simple3DRenderObj *renderObj, const VecFx32 *position);

BOOL ov01_021FE4FC(OverworldAnimManager *manager, Ov01_021FE3F8_Work *work) {
    const Ov01_021FE3F8_Args *args;

    work->kind = sub_02068D90(manager);
    args = sub_02068D98(manager);
    work->renderObj = args->renderObj;
    work->model = args->model;
    work->alpha = 0x1F;
    work->kind = args->kind;
    return 1;
}

void ov01_021FE524(void) {
}

void ov01_021FE528(OverworldAnimManager *manager, Ov01_021FE3F8_Work *work) {
    s32 v0;
    s32 v1;
    s32 v2;

    v1 = work->state;
    switch (v1) {
    case 0:
        v0 = work->timer + 1;
        work->timer = v0;
        if (v0 >= 0x10) {
            work->state = (s32)(work->state + 1);
            return;
        }
        return;
    case 1:
        v2 = work->alpha - 2;
        work->alpha = v2;
        if (v2 < 0) {
            ov01_021F1640(manager);
        }
        break;
    }
}

void ov01_021FE558(OverworldAnimManager *manager, Ov01_021FE3F8_Work *work) {
    VecFx32 pos;

    if (work->unk_08 == 0) {
        sub_02068DB8(manager, &pos);
        NNSi_G3dModifyPolygonAttrMask(work->model->g3DModel, 1, 0x1F0000U);
        NNS_G3dMdlSetMdlAlphaAll(work->model->g3DModel, work->alpha);
        sub_020699BC(work->renderObj, &pos);
    }
}
