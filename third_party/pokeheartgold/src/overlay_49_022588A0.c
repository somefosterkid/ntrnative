#include "global.h"
#include "field/overlay_01_021FB878.h"

struct Field3dModel;

struct Field3dObject;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

void sub_020182B0(const Field3dObject *obj, fx32 *outX, fx32 *outY, fx32 *outZ);

void sub_020182CC(const Field3dObject *obj, fx32 *outX, fx32 *outY, fx32 *outZ);

u16 sub_020182EC(const Field3dObject *obj, enum RotationAxis axis);

u32 sub_0201F990(const NNSG3dResMdl *model, const VecFx32 *position, const MtxFx33 *rotation, const VecFx32 *scale);

BOOL ov49_022588A0(const Field3dModel *param0, const Field3dObject *param1);

BOOL ov49_022588A0(const Field3dModel *param0, const Field3dObject *param1) {
    VecFx32 v0;
    VecFx32 v1;
    MtxFx33 v2;
    MtxFx33 v3;
    u32 v4;
    u32 v5;
    u32 v6;

    sub_020182B0(param1, &v0.x, &v0.y, &v0.z);
    sub_020182CC(param1, &v1.x, &v1.y, &v1.z);

    v4 = sub_020182EC(param1, (enum RotationAxis)(0));
    v5 = sub_020182EC(param1, (enum RotationAxis)(1));
    v6 = sub_020182EC(param1, (enum RotationAxis)(2));

    MTX_Identity33(&v2);
    MTX_RotX33(&v3, FX_SinIdx(v4), FX_CosIdx(v4));
    MTX_Concat33(&v3, &v2, &v2);
    MTX_RotZ33(&v3, FX_SinIdx(v6), FX_CosIdx(v6));
    MTX_Concat33(&v3, &v2, &v2);
    MTX_RotY33(&v3, FX_SinIdx(v5), FX_CosIdx(v5));
    MTX_Concat33(&v3, &v2, &v2);

    return sub_0201F990(param0->model, &v0, &v2, &v1);
}
