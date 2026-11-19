#include "global.h"

#include "gf_3d_render.h"
#include "unk_02018000.h"
#include "constants/heap.h"
#include "field/overlay_01_021FB878.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "sys_task.h"
#include "sys_task_api.h"

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

typedef struct Easy3DObject {
    NNSG3dRenderObj renderObj;
    VecFx32 position;
    VecFx32 scale;
    BOOL visible;
    u16 rotation[ROTATION_AXIS_COUNT];
    u8 padding_76[2];
} Easy3DObject;

void sub_020181EC(Easy3DObject *obj);

void sub_020182E0(Field3dObject *arg0, s16 arg1, s32 arg2);

void sub_0201804C(SysTask *task, void *param);
void sub_020182F8(Field3DModelAnimation *anim, const Field3dModel *model, void *data, NNSFndAllocator *allocator);
void sub_02018324(Field3dModel *model);

void sub_020181EC(Easy3DObject *obj) {
    MtxFx33 rotation;
    MtxFx33 temp;

    if (obj->visible) {
        MTX_Identity33(&rotation);
        MTX_RotX33(&temp, FX_SinIdx(obj->rotation[ROTATION_AXIS_X]), FX_CosIdx(obj->rotation[ROTATION_AXIS_X]));
        MTX_Concat33(&temp, &rotation, &rotation);
        MTX_RotZ33(&temp, FX_SinIdx(obj->rotation[ROTATION_AXIS_Z]), FX_CosIdx(obj->rotation[ROTATION_AXIS_Z]));
        MTX_Concat33(&temp, &rotation, &rotation);
        MTX_RotY33(&temp, FX_SinIdx(obj->rotation[ROTATION_AXIS_Y]), FX_CosIdx(obj->rotation[ROTATION_AXIS_Y]));
        MTX_Concat33(&temp, &rotation, &rotation);

        GF3dRender_DrawModel(&obj->renderObj, &obj->position, &rotation, &obj->scale);
    }
}

void sub_02018288(Easy3DObject *obj, const MtxFx33 *rotation);

void sub_02018288(Easy3DObject *obj, const MtxFx33 *rotation) {
    if (obj->visible) {
        GF3dRender_DrawModel(&obj->renderObj, &obj->position, (MtxFx33 *)rotation, &obj->scale);
    }
}

void sub_020182A0(void *arg0, s32 arg1) {
    *((s32 *)&((Easy3DObject *)arg0)->visible) = arg1;
}

s32 sub_020182A4(void *arg0) {
    return *((s32 *)&((Easy3DObject *)arg0)->visible);
}

void sub_020182A8(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    *((s32 *)&((Easy3DObject *)arg0)->position.x) = arg1;
    *((s32 *)&((Easy3DObject *)arg0)->position.y) = arg2;
    *((s32 *)&((Easy3DObject *)arg0)->position.z) = arg3;
}

void sub_020182B0(void *arg0, s32 *arg1, s32 *arg2, s32 *arg3) {
    *arg1 = *((s32 *)&((Easy3DObject *)arg0)->position.x);
    *arg2 = *((s32 *)&((Easy3DObject *)arg0)->position.y);
    *arg3 = *((s32 *)&((Easy3DObject *)arg0)->position.z);
}

void sub_020182C4(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    *((s32 *)&((Easy3DObject *)arg0)->scale.x) = arg1;
    *((s32 *)&((Easy3DObject *)arg0)->scale.y) = arg2;
    *((s32 *)&((Easy3DObject *)arg0)->scale.z) = arg3;
}

void sub_020182CC(void *arg0, s32 *arg1, s32 *arg2, s32 *arg3) {
    *arg1 = *((s32 *)&((Easy3DObject *)arg0)->scale.x);
    *arg2 = *((s32 *)&((Easy3DObject *)arg0)->scale.y);
    *arg3 = *((s32 *)&((Easy3DObject *)arg0)->scale.z);
}

void sub_020182E0(Field3dObject *arg0, s16 arg1, s32 arg2) {
    arg0->rotation[arg2] = arg1;
}

u16 sub_020182EC(const Field3dObject *arg0, s32 arg1) {
    return arg0->rotation[arg1];
}

void sub_020182F8(Field3DModelAnimation *anim, const Field3dModel *model, void *data, NNSFndAllocator *allocator) {
    anim->fileDataRaw = data;
    anim->animRaw = NNS_G3dGetAnmByIdx(anim->fileDataRaw, 0);
    anim->animObj = NNS_G3dAllocAnmObj(allocator, anim->animRaw, model->model);

    NNS_G3dAnmObjInit(anim->animObj, anim->animRaw, model->model, model->texture);
}

void sub_02018324(Field3dModel *model) {
    ((model->dataRaw) ? (void)0 : GF_AssertFail());

    model->modelSet = NNS_G3dGetMdlSet(model->dataRaw);
    model->model = NNS_G3dGetMdlByIdx(model->modelSet, 0);
    model->texture = NNS_G3dGetTex(model->dataRaw);

    if (model->texture) {
        SysTask_CreateOnVWaitQueue(sub_0201804C, model, 1024);
    }
}
