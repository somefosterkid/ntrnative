#include "global.h"

#include "camera.h"
#include "overlay_108_021E8850.h"
#include "overlay_108_safari_decoration_app.h"

extern CameraAngle ov108_021EA9C4;
extern VecFx32 ov108_021EA9D8;

void ov108_021E91F8(SafariDecoration3dModel *model, SafariDecoration3dAnim *anim) {
    NNS_G3dRenderObjAddAnmObj(&model->renderObj, anim->anmObj);
    return;
}

void ov108_021E9204(SafariDecoration3dModel *model, SafariDecoration3dAnim *anim) {
    NNS_G3dRenderObjRemoveAnmObj(&model->renderObj, anim->anmObj);
    return;
}

void ov108_021E9210(SafariDecoration3dAnim *anim) {
    s32 v0;

    v0 = (s32)(anim->frame + 0x1000) % (s32)(((NNSG3dResJntAnm *)anim->anmObj->resAnm)->numFrame << 0xC);
    anim->frame = v0;
    anim->anmObj->frame = v0;
}

void ov108_021E9230(SafariDecoration3dView *view, enum HeapID heapID) {
    view->camera = Camera_New(heapID);
    view->target.x = 0;
    view->target.y = 0;
    view->target.z = 0;
    Camera_Init_FromTargetDistanceAndAngle(&view->target, 0x15B000, &ov108_021EA9C4, 0x5C1U, 0U, 1, view->camera);
    Camera_SetStaticPtr(view->camera);
    Camera_SetPerspectiveClippingPlane(0x64000, 0x4B0000, view->camera);
    Camera_OffsetLookAtPosAndTarget(&ov108_021EA9D8, view->camera);
}
