#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_3d_render.h"

void Field3dModelAnimation_FrameSet(Field3DModelAnimation *anim, fx32 frame) {
    anim->frame = frame;
    NNS_G3dAnmObjSetFrame(anim->animObj, frame);
}

fx32 Field3dModelAnimation_FrameGet(Field3DModelAnimation *anim) {
    return anim->frame;
}

void Field3dObject_InitFromModel(Field3dObject *obj, Field3dModel *model) {
    memset(obj, 0, sizeof(Field3dObject));
    NNS_G3dRenderObjInit(&obj->renderObj, model->model);

    obj->active = 1;
    obj->scale.x = FX32_ONE;
    obj->scale.y = FX32_ONE;
    obj->scale.z = FX32_ONE;
}

void Field3dObject_AddAnimation(Field3dObject *obj, Field3DModelAnimation *anim) {
    NNS_G3dRenderObjAddAnmObj(&obj->renderObj, anim->animObj);
}

void Field3dObject_RemoveAnimation(Field3dObject *obj, Field3DModelAnimation *anim) {
    NNS_G3dRenderObjRemoveAnmObj(&obj->renderObj, anim->animObj);
}

void Field3dObject_Draw(Field3dObject *obj) {
    MtxFx33 rotation;
    MtxFx33 temp;

    if (obj->active) {
        MTX_Identity33(&rotation);
        MTX_RotX33(&temp, FX_SinIdx(obj->rotation[0]), FX_CosIdx(obj->rotation[0]));
        MTX_Concat33(&temp, &rotation, &rotation);
        MTX_RotZ33(&temp, FX_SinIdx(obj->rotation[2]), FX_CosIdx(obj->rotation[2]));
        MTX_Concat33(&temp, &rotation, &rotation);
        MTX_RotY33(&temp, FX_SinIdx(obj->rotation[1]), FX_CosIdx(obj->rotation[1]));
        MTX_Concat33(&temp, &rotation, &rotation);

        GF3dRender_DrawModel(&obj->renderObj, &obj->matrix, &rotation, &obj->scale);
    }
}

void Field3dObject_SetActiveFlag(Field3dObject *obj, BOOL active) {
    obj->active = active;
}

BOOL Field3dObject_GetActiveFlag(Field3dObject *obj) {
    return obj->active;
}

void Field3dObject_SetPosEx(Field3dObject *obj, fx32 x, fx32 y, fx32 z) {
    obj->matrix.x = x;
    obj->matrix.y = y;
    obj->matrix.z = z;
}
