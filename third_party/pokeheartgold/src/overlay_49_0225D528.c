#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "filesystem.h"
#include "gf_3d_render.h"
#include "overlay_49.h"

void ov49_02258830(void **param0, NARC *param1, u32 param2, u32 heapID);

void sub_02018068(Field3dModel *model);
void ov49_0225D574(Field3dModel *model);
s32 sub_020181A4(void *);
void ov49_0225D57C(s32 *arg0, void *arg1, s32 arg2);
s32 ov49_0225D5A0(s32 *arg0, void *arg1, s32 arg2);
void ov49_0225D5C8(s32 *arg0, void *arg1, s32 arg2);

struct Field3DModelAnimation;
BOOL ov49_0225D5E4(fx32 *param0, const Field3DModelAnimation *param1, fx32 param2);

void ov49_0225D528(Field3dModel *param0, NARC *param1, u32 param2, u32 heapID) {
    ov49_02258830(&param0->dataRaw, param1, param2, heapID);

    {
        param0->modelSet = NNS_G3dGetMdlSet(param0->dataRaw);
        param0->model = NNS_G3dGetMdlByIdx(param0->modelSet, 0);
        param0->texture = NNS_G3dGetTex(param0->dataRaw);
    }

    {
        GF3dRender_BindModelSet(param0->dataRaw, param0->texture);
    }
}

void ov49_0225D574(Field3dModel *model) {
    sub_02018068(model);
}

void ov49_0225D57C(s32 *arg0, void *arg1, s32 arg2) {
    s32 v0;
    s32 v1;

    v0 = sub_020181A4(arg1);
    v1 = *arg0 + arg2;
    if (v1 < v0) {
        *arg0 = v1;
        return;
    }
    *arg0 = v1 % v0;
}

s32 ov49_0225D5A0(s32 *arg0, void *arg1, s32 arg2) {
    s32 v0;
    s32 v1;

    v0 = sub_020181A4(arg1);
    v1 = *arg0 + arg2;
    if (v1 < v0) {
        *arg0 = v1;
        return 0;
    }
    *arg0 = v0 - 0x800;
    return 1;
}

void ov49_0225D5C8(s32 *arg0, void *arg1, s32 arg2) {
    s32 v0;
    s32 v1;

    v0 = sub_020181A4(arg1);
    v1 = *arg0 - arg2;
    if (v1 >= 0) {
        *arg0 = v1;
        return;
    }
    *arg0 = v0 + v1;
}

BOOL ov49_0225D5E4(fx32 *param0, const Field3DModelAnimation *param1, fx32 param2) {
    BOOL v0;

    if (((*param0) - param2) > 0) {
        (*param0) -= param2;
        v0 = 0;
    } else {
        (*param0) = 0;
        v0 = 1;
    }

    return v0;
}
