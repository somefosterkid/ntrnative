#include "global.h"

#include "unk_02018000.h"
#include "constants/heap.h"
#include "field/overlay_01_021FB878.h"
#include "filesystem.h"
#include "filesystem_files_def.h"


enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
void sub_020181B0(Field3dObject *obj, Field3dModel *model);

s32 sub_020181A4(void *arg0) {
    return *(u16 *)((u8 *)(*(void **)((u8 *)(*(void **)((u8 *)arg0 + 8)) + 8)) + 4) << 0xC;
}

void sub_020181B0(Field3dObject *obj, Field3dModel *model) {
    memset(obj, 0, sizeof(Field3dObject));
    NNS_G3dRenderObjInit(&obj->renderObj, model->model);

    obj->active = 1;
    obj->scale.x = ((fx32)0x0000000000001000L);
    obj->scale.y = ((fx32)0x0000000000001000L);
    obj->scale.z = ((fx32)0x0000000000001000L);
}

void sub_020181D4(NNSG3dRenderObj *pRenderObj, void *arg1) {
    NNS_G3dRenderObjAddAnmObj(pRenderObj, *(NNSG3dAnmObj **)((u8 *)arg1 + 8));
}

void sub_020181E0(NNSG3dRenderObj *pRenderObj, void *arg1) {
    NNS_G3dRenderObjRemoveAnmObj(pRenderObj, ((Field3DModelAnimation *)arg1)->animObj);
}
