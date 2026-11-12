// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/calctool.c, less vecfx_div, which Black
// doesn't have.
#include "types.h"
#include "gfl/calctool.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/fx/fx_mtx.h"

void MAT2_SetScaleRot(MtxFx22 *mtx, u16 rotation, fx32 scaleX, fx32 scaleY, u8 rotationMode) {
    if (rotationMode == MAT2_ROT_256) {
        rotation = (rotation * 0xffff) >> 8;
    } else if (rotationMode == MAT2_ROT_DEG) {
        rotation = (u32)(rotation * 0xffff) / 360;
    }
    MTX_Rot22_(mtx, FX_SinIdx(rotation), FX_CosIdx(rotation));
    MTX_ScaleApply22(mtx, mtx, FX_Inv(scaleX), FX_Inv(scaleY));
}

void func_02051f1c(CalcSphere *sphere, const VecFx32 *center, fx32 radius) {
    sphere->center = *center;
    sphere->radius = radius;
}

BOOL func_02051f34(const CalcCapsule *capsule, const CalcSphere *sphere, CalcHitResult *result) {
    CalcHitResult hit;
    VecFx32 segment;
    VecFx32 toCenter;
    VecFx32 diff;
    fx32 t;

    hit.hit = FALSE;
    VEC_Subtract(&capsule->end, &capsule->start, &segment);
    VEC_Subtract(&sphere->center, &capsule->start, &toCenter);
    t = FX_Div(VEC_DotProduct(&segment, &toCenter), VEC_DotProduct(&segment, &segment));
    if (t >= 0 && t <= FX32_ONE) {
        // The nearest point is inside the segment
        vecfx_mul(&segment, t, &segment);
        VEC_Subtract(&segment, &toCenter, &diff);
        hit.dist = VEC_Mag(&diff);
        if (hit.dist <= sphere->radius + capsule->radius) {
            hit.hit = TRUE;
        }
    } else {
        // The nearest point is an end
        VEC_Subtract(&sphere->center, &capsule->end, &diff);
        hit.dist = VEC_Mag(&diff);
        if (hit.dist <= sphere->radius) {
            hit.hit = TRUE;
        } else {
            diff = toCenter;
            hit.dist = VEC_Mag(&diff);
            if (hit.dist <= sphere->radius) {
                hit.hit = TRUE;
            }
        }
    }
    if (result != NULL) {
        VEC_Normalize(&diff, &hit.dir);
        *result = hit;
    }
    return hit.hit;
}

void func_0205207c(CalcCapsule *capsule, const VecFx32 *start, const VecFx32 *end, fx32 radius) {
    capsule->start = *start;
    capsule->end = *end;
    capsule->radius = radius;
}

void vecfx_mul(const VecFx32 *v, fx32 scale, VecFx32 *dest) {
    dest->x = FX_Mul(v->x, scale);
    dest->y = FX_Mul(v->y, scale);
    dest->z = FX_Mul(v->z, scale);
}

void MAT3_RotationEulerZYX(u16 x, u16 y, u16 z, MtxFx33 *mtx) {
    MtxFx33 rotation;

    MTX_Identity33_(mtx);
    MTX_RotX33_(mtx, FX_SinIdx(x), FX_CosIdx(x));
    MTX_RotY33_(&rotation, FX_SinIdx(y), FX_CosIdx(y));
    MTX_Concat33(mtx, &rotation, mtx);
    MTX_RotZ33_(&rotation, FX_SinIdx(z), FX_CosIdx(z));
    MTX_Concat33(mtx, &rotation, mtx);
}

void MAT3_RotationDir(const VecFx32 *dir, MtxFx33 *mtx) {
    VecFx32 flat;
    MtxFx33 rotationX;
    MtxFx33 rotationY;
    u16 angle;

    flat = *dir;
    flat.y = 0;
    angle = FX_Atan2Idx(-dir->y, VEC_Mag(&flat));
    MTX_RotX33_(&rotationX, FX_SinIdx(angle), FX_CosIdx(angle));
    angle = FX_Atan2Idx(dir->x, dir->z);
    MTX_RotY33_(&rotationY, FX_SinIdx(angle), FX_CosIdx(angle));
    MTX_Concat33(&rotationX, &rotationY, mtx);
}
