// Black's code at ov021 0x0218e018-0x0218f0d4. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field_camera.h"
#include "field/ov021_0218f0d4.h"
#include "gfl/arc.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_vec.h"
#include "struct_decls.h"
#include "nitro/math.h"
#include "gfl/arc_util.h"
#include "gfl/std.h"


BOOL func_ov021_0218f1fc(const FieldCameraBoundaryEntry *entry, const VecFx32 *in, VecFx32 *out);
BOOL func_ov021_0218f310(const FieldCameraBoundaryEntry *entry, const VecFx32 *in, VecFx32 *out);
BOOL func_ov021_0218f460(const FieldCameraBoundaryEntry *entry, const VecFx32 *in, VecFx32 *out);

// The camera's delay (pokebw2's): the last count targets and eyes in two rings, the camera following maxDelay behind
struct FieldCameraDelay {
    s32 count;
    s32 head;
    s32 tail;
    s32 maxDelay;
    BOOL filled;
    BOOL delayX;
    BOOL delayY;
    BOOL delayZ;
    VecFx32 *targets;
    VecFx32 *eyes;
    BOOL active;
    BOOL finishing;
};

void FieldCamera_FreeDelayManager(FieldCamera *camera);
void FieldCamera_CoordsLoadDefaults(FieldCamera *camera, const VecFx32 *target);
void FieldCameraApply_SetInitTgtBase(FieldCamera *camera);
void FieldCameraApply_MakeEyeByTarget(FieldCamera *camera);
void func_ov021_0218e540(FieldCamera *camera);
u32 FieldCameraApply_SetInitEyeBase(FieldCamera *camera);
void FieldCameraApply_MakeTargetByEye(FieldCamera *camera);
void func_ov021_0218e4b0(FieldCamera *camera);
void FieldCamera_CoordsDerive_(VecFx32 *param_1, u16 *param_2, u16 *param_3, fx32 *param_4);

void FieldCamera_CallTransformCalcFunc(FieldCamera *camera, u32 keys);
void func_ov021_0218e5a0(FieldCamera *camera, u32 keys);
void func_ov021_0218e5b8(FieldCamera *camera, u32 keys);
void func_ov021_0218e5d0(FieldCamera *camera, u32 keys);
void func_ov021_0218e5ec(FieldCamera *camera);
void func_ov021_0218e5f4(FieldCamera *camera);
void func_ov021_0218e5fc(FieldCamera *camera);
void func_ov021_0218e6a4(FieldCamera *camera);
void func_ov021_0218e6f0(FieldCamera *camera);
void func_ov021_0218e73c(FieldCamera *camera);
void FieldCamera_ApplyDelay(FieldCamera *camera);
void func_ov021_0218e764(FieldCamera *camera, VecFx32 *param_2, VecFx32 *param_3);
void func_ov021_0218e7dc(const VecFx32 *dir, u16 yaw, u16 pitch, u16 *outYaw, u16 *outPitch);

void func_ov021_0218eefc(s32 param_1, s32 param_2, u32 param_3, u32 heap, FieldCamera *camera);
void FieldCameraDelay_Update(FieldCameraDelay *delay, VecFx32 *param_2, VecFx32 *param_3, VecFx32 *param_4, VecFx32 *param_5);
void FieldCameraDelay_Disable(FieldCameraDelay *delay);

void (*FIELD_CAMERA_TRANSFORM_INIT_FUNCS[])(FieldCamera *) = {
    func_ov021_0218e5ec,
    func_ov021_0218e5f4,
    func_ov021_0218e5fc,
};

void (*FIELD_CAMERA_TRANSFORM_CONVERT_FUNCS[])(FieldCamera *) = {
    func_ov021_0218e6a4,
    func_ov021_0218e6f0,
    func_ov021_0218e73c,
};

void (*const FIELD_CAMERA_TRANSFORM_FUNCS[])(FieldCamera *, u32) = {
    func_ov021_0218e5a0,
    func_ov021_0218e5b8,
    func_ov021_0218e5d0,
};


void FieldCamera_SetZFar(FieldCamera *camera, fx32 zFar);
void FieldCamera_SetZNear(FieldCamera *camera, fx32 zNear);


FieldCamera *func_ov021_0218e018(u32 param_1, u32 param_2, G3DCamera *camera, VecFx32 *param_4, HeapID heap) {
    FieldCamera *cam;
    fx32 fovCos;
    cam = GFL_HeapAllocate(heap, sizeof(FieldCamera), TRUE, "field_camera.c", 0x80);
    cam->defaultsIndex = param_1;
    cam->transformType = param_2;
    cam->g3dCamera = camera;
    cam->eyeBind = NULL;
    cam->heapId = heap;
    cam->eye.x = 0;
    cam->eye.y = 0;
    cam->eye.z = 0;
    cam->target.x = 0;
    cam->target.y = 0;
    cam->target.z = 0;
    cam->targetBase.x = 0;
    cam->targetBase.y = 0;
    cam->targetBase.z = 0;
    cam->eyeOffset.x = 0;
    cam->eyeOffset.y = 0;
    cam->eyeOffset.z = 0;
    cam->yaw = 0;
    cam->pitch = 0;
    cam->zoom = 0x78;
    cam->useBoundaryEnable = TRUE;
    if (GFL_G3DCameraGetProjectionType(camera) == G3DCAM_PROJECTION_PERSPECTIVE) {
        GFL_G3DCameraPerspectiveGetFOVCos(camera, &fovCos);
        cam->fov = FX_AcosIdx(fovCos);
    }
    cam->defaultsArc = GFL_ArcSysCreateFileHandle(0x3c, cam->heapId);
    cam->boundaryArc = GFL_ArcSysCreateFileHandle(0x6c, cam->heapId);
    cam->boundaryId = 0xffff;
    FieldCamera_CoordsLoadDefaults(cam, param_4);
    func_ov021_0218eefc(7, 6, 2, heap, cam);
    cam->anm.callback = NULL;
    return cam;
}

void func_ov021_0218e0e4(FieldCamera *camera) {
    GFL_ArcToolFree(camera->defaultsArc);
    GFL_ArcToolFree(camera->boundaryArc);
    FieldCamera_FreeDelayManager(camera);
    GFL_HeapFree(camera);
}

void FieldCamera_CalcTransform(FieldCamera *camera, u16 keys) {
    if (camera->anm.callback != NULL) {
        camera->anm.callback(camera, camera->anm.calcArg);
    }
    FieldCamera_CallTransformCalcFunc(camera, keys);
}

void FieldCamera_SetTransformType(FieldCamera *camera, u32 type) {
    camera->transformType = type;
    FIELD_CAMERA_TRANSFORM_INIT_FUNCS[type](camera);
}


u32 FieldCamera_GetTransformType(FieldCamera *camera) {
    return camera->transformType;
}

void FieldCamera_ChangeTransformType(FieldCamera *camera, u32 type) {
    if (camera->transformType != type) {
        FIELD_CAMERA_TRANSFORM_CONVERT_FUNCS[type](camera);
        FieldCamera_SetTransformType(camera, type);
    }
}


void FieldCamera_LoadDefaults(FieldCamera *camera) {
    FieldCamera_SetTransformType(camera, 0);
    FieldCamera_CoordsLoadDefaults(camera, camera->refBind);
    camera->useBoundaryEnable = TRUE;
}

void FieldCamera_SetDefaultsIndex(FieldCamera *camera, u32 index) {
    camera->defaultsIndex = index;
    FieldCamera_LoadDefaults(camera);
}

u32 func_ov021_0218e188(FieldCamera *camera) {
    return camera->defaultsIndex;
}

void FieldCamera_CoordsLoadDefaults(FieldCamera *camera, const VecFx32 *target) {
    struct FieldCameraDefaults setup;
    FieldCamera_ReadDefaults(camera, &setup);
    camera->zoom = setup.distance << 12;
    camera->pitch = setup.pitch;
    camera->yaw = setup.yaw;
    camera->fov = setup.fov;
    FieldCamera_SetZFar(camera, setup.zFar);
    FieldCamera_SetZNear(camera, setup.zNear);
    switch (setup.bufferMode) {
    case 0:
        GFL_G3DSysSetSwapBufferParams(0, 0);
        break;
    case 1:
        GFL_G3DSysSetSwapBufferParams(0, 1);
        break;
    }
    if (GFL_G3DCameraGetProjectionType(camera->g3dCamera) == G3DCAM_PROJECTION_PERSPECTIVE) {
        GFL_G3DCameraPerspectiveSetFOVSin(camera->g3dCamera, FX_SinCosTable_[(camera->fov >> 4) * 2]);
        GFL_G3DCameraPerspectiveSetFOVCos(camera->g3dCamera, FX_SinCosTable_[(camera->fov >> 4) * 2 + 1]);
    }
    if (setup.bind != 0) {
        camera->refBind = target;
        camera->bind = target;
        camera->target = *target;
        camera->targetOffset = setup.target;
    } else {
        camera->refBind = NULL;
        camera->bind = NULL;
        camera->target = setup.target;
        camera->targetOffset.x = 0;
        camera->targetOffset.y = 0;
        camera->targetOffset.z = 0;
    }
}


void FieldCamera_ReadDefaults(FieldCamera *camera, struct FieldCameraDefaults *defaults) {
    GFL_ArcToolGetDataLength(camera->defaultsArc, 0);
    GFL_ArcToolReadRange(camera->defaultsArc, 0, camera->defaultsIndex * sizeof(struct FieldCameraDefaults),
                         sizeof(struct FieldCameraDefaults), defaults);
}

void FieldCameraApply_SetInitTgtBase(FieldCamera *camera) {
    const VecFx32 *bind = camera->bind;
    if (bind != NULL) {
        camera->target = *bind;
    }
}

u32 FieldCameraApply_SetInitEyeBase(FieldCamera *camera) {
    const VecFx32 *src = camera->eyeBind;
    if (src != NULL) {
        camera->eye = *src;
        return 1;
    }
    return 0;
}

void FieldCameraApply_TransRot(VecFx32 *param_1, VecFx32 *param_2, s32 param_3, s32 param_4, fx32 param_5) {
    VecFx32 v;
    s32 s0 = FX_SinCosTable_[(param_3 >> 4) * 2];
    s32 c0 = FX_SinCosTable_[(param_3 >> 4) * 2 + 1];
    s32 s1 = FX_SinCosTable_[(param_4 >> 4) * 2];
    s16 c1 = FX_SinCosTable_[(param_4 >> 4) * 2 + 1];
    if (c1 < 0) {
        c1 = -c1;
    }
    if (c1 < 0x200) {
        c1 = 0x200;
    }
    v.x = (fx32)((((u64)s0 * c1) + 0x800) >> 12);
    v.y = s1;
    v.z = (fx32)((((u64)c0 * c1) + 0x800) >> 12);
    VEC_Normalize(&v, &v);
    VEC_MultAdd(param_5, &v, param_1, &v);
    *param_2 = v;
}

void FieldCameraApply_MakeEyeByTarget(FieldCamera *camera) {
    s32 i;
    camera->targetActual = camera->target;
    if (camera->useBoundaryEnable != 0) {
        for (i = 0; i < camera->boundaryCount; i++) {
            FieldCameraBoundary_ApplyTarget(&camera->boundary[i], &camera->targetActual);
        }
    }
    FieldCameraApply_TransRot(&camera->targetActual, &camera->eye, camera->yaw, camera->pitch, camera->zoom);
    camera->eyeActual = camera->eye;
    if (camera->useBoundaryEnable != 0) {
        for (i = 0; i < camera->boundaryCount; i++) {
            FieldCameraBoundary_ApplyEye(&camera->boundary[i], &camera->eyeActual);
        }
    }
}

void FieldCameraApply_MakeTargetByEye(FieldCamera *camera) {
    s32 i;
    camera->eyeActual = camera->eye;
    if (camera->useBoundaryEnable != 0) {
        for (i = 0; i < camera->boundaryCount; i++) {
            FieldCameraBoundary_ApplyEye(&camera->boundary[i], &camera->eyeActual);
        }
    }
    FieldCameraApply_TransRot(&camera->eyeActual, &camera->target, camera->yaw, camera->pitch, camera->zoom);
    camera->targetActual = camera->target;
    if (camera->useBoundaryEnable != 0) {
        for (i = 0; i < camera->boundaryCount; i++) {
            FieldCameraBoundary_ApplyTarget(&camera->boundary[i], &camera->targetActual);
        }
    }
}

void func_ov021_0218e4b0(FieldCamera *camera) {
    s32 i;
    camera->eyeActual = camera->eye;
    if (camera->useBoundaryEnable != 0) {
        for (i = 0; i < camera->boundaryCount; i++) {
            FieldCameraBoundary_ApplyEye(&camera->boundary[i], &camera->eyeActual);
        }
    }
    camera->targetActual = camera->target;
    if (camera->useBoundaryEnable != 0) {
        for (i = 0; i < camera->boundaryCount; i++) {
            FieldCameraBoundary_ApplyTarget(&camera->boundary[i], &camera->targetActual);
        }
    }
}





void func_ov021_0218e540(FieldCamera *camera) {
    VecFx32 target;
    VecFx32 pos;
    VEC_Add(&camera->targetActual, &camera->targetOffset, &target);
    VEC_Add(&camera->eyeActual, &camera->eyeOffset, &pos);
    GFL_G3DCameraSetLookatTarget(camera->g3dCamera, &target);
    GFL_G3DCameraSetLookatPos(camera->g3dCamera, &pos);
    FieldCamera_ApplyDelay(camera);
    func_ov021_0218e764(camera, &target, &camera->eyeActual);
}



void FieldCamera_CallTransformCalcFunc(FieldCamera *camera, u32 keys) {
    FIELD_CAMERA_TRANSFORM_FUNCS[camera->transformType](camera, keys);
}



void func_ov021_0218e5a0(FieldCamera *camera, u32 keys) {
    FieldCameraApply_SetInitTgtBase(camera);
    FieldCameraApply_MakeEyeByTarget(camera);
    func_ov021_0218e540(camera);
}

void func_ov021_0218e5b8(FieldCamera *camera, u32 keys) {
    FieldCameraApply_SetInitEyeBase(camera);
    FieldCameraApply_MakeTargetByEye(camera);
    func_ov021_0218e540(camera);
}

void func_ov021_0218e5d0(FieldCamera *camera, u32 keys) {
    FieldCameraApply_SetInitTgtBase(camera);
    FieldCameraApply_SetInitEyeBase(camera);
    func_ov021_0218e4b0(camera);
    func_ov021_0218e540(camera);
}

void func_ov021_0218e5ec(FieldCamera *camera) {
    camera->eyeBind = NULL;
}

void func_ov021_0218e5f4(FieldCamera *camera) {
    camera->bind = NULL;
}

void func_ov021_0218e5fc(FieldCamera *camera) {
    camera->pitch = 0;
    camera->yaw = 0;
    camera->zoom = 0;
}

void FieldCamera_CoordsDerive_(VecFx32 *param_1, u16 *param_2, u16 *param_3, fx32 *param_4) {
    fx32 mag;
    VecFx32 norm;
    VecFx32 flat;
    u16 pitch;
    u16 yaw;
    mag = VEC_Mag(param_1);
    VEC_Normalize(param_1, &norm);
    flat.x = norm.x;
    flat.y = 0;
    flat.z = norm.z;
    pitch = FX_Atan2Idx(norm.y, VEC_Mag(&flat));
    yaw = FX_Atan2Idx(norm.x, norm.z);
    func_ov021_0218e7dc(&norm, yaw, pitch, &yaw, &pitch);
    *param_2 = pitch;
    *param_3 = yaw;
    *param_4 = mag;
}


void FieldCamera_CoordsSetFromDir(FieldCamera *camera, VecFx32 *param_2) {
    u16 pitch;
    u16 yaw;
    fx32 zoom;
    FieldCamera_CoordsDerive_(param_2, &pitch, &yaw, &zoom);
    camera->pitch = pitch;
    camera->yaw = yaw;
    camera->zoom = zoom;
}

void func_ov021_0218e6a4(FieldCamera *camera) {
    VecFx32 diff;
    if (camera->transformType == 1) {
        camera->yaw = camera->yaw + 0x8000;
        camera->pitch = camera->pitch + 0x8000;
        return;
    }
    VEC_Subtract(&camera->eye, &camera->targetActual, &diff);
    FieldCamera_CoordsSetFromDir(camera, &diff);
}

void func_ov021_0218e6f0(FieldCamera *camera) {
    VecFx32 diff;
    if (camera->transformType == 0) {
        camera->yaw = camera->yaw + 0x8000;
        camera->pitch = camera->pitch + 0x8000;
        return;
    }
    VEC_Subtract(&camera->target, &camera->eyeActual, &diff);
    FieldCamera_CoordsSetFromDir(camera, &diff);
}




void func_ov021_0218e73c(FieldCamera *camera) {
    if (camera->transformType == 0) {
        FieldCameraApply_SetInitTgtBase(camera);
        FieldCameraApply_MakeEyeByTarget(camera);
        return;
    }
    if (camera->transformType == 1) {
        FieldCameraApply_SetInitEyeBase(camera);
        FieldCameraApply_MakeTargetByEye(camera);
    }
}

static inline VecFx32 FieldCamera_AxisZ(void) {
    VecFx32 axis = { 0, 0, FX32_ONE };
    return axis;
}

void func_ov021_0218e764(FieldCamera *camera, VecFx32 *param_2, VecFx32 *param_3) {
    VecFx32 dir;
    VEC_Subtract(param_3, param_2, &dir);
    VEC_Normalize(&dir, &dir);
    camera->angleYaw = FX_Atan2Idx(dir.x, dir.z);
    {
        VecFx32 up = { 0, FX32_ONE, 0 };
        VecFx32 flat;
        fx32 dot;
        fx32 x;
        u16 pitch;
        dot = VEC_DotProduct(&up, &dir);
        flat = dir;
        pitch = 0;
        flat.y = 0;
        VEC_Normalize(&flat, &flat);
        x = VEC_DotProduct(&flat, &dir);
        if (x != 0) {
            pitch = FX_Atan2Idx(dot, x);
        }
        camera->anglePitch = pitch;
    }
}

void func_ov021_0218e7dc(const VecFx32 *dir, u16 yaw, u16 pitch, u16 *outYaw, u16 *outPitch) {
    VecFx16 base = { 0, 0, 0 };
    VecFx16 minus = { 0, 0, 0 };
    VecFx16 plus = { 0, 0, 0 };
    fx16 sinYaw;
    fx16 cosYaw;
    fx16 cosPitch;
    fx16 sinPitchMinus;
    fx16 sinPitchPlus;
    *outYaw = yaw;
    *outPitch = pitch;
    sinYaw = FX_SinIdx(yaw);
    cosYaw = FX_CosIdx(yaw);
    cosPitch = FX_CosIdx(pitch);
    base.x = FX_Mul(sinYaw, cosPitch);
    base.y = FX_SinIdx(pitch);
    base.z = FX_Mul(cosYaw, cosPitch);
    minus.x = FX_Mul(FX_SinIdx(yaw - DEG_TO_IDX(1)), cosPitch);
    minus.z = FX_Mul(FX_CosIdx(yaw - DEG_TO_IDX(1)), cosPitch);
    sinPitchMinus = FX_SinIdx(pitch - DEG_TO_IDX(1));
    plus.x = FX_Mul(FX_SinIdx(yaw + DEG_TO_IDX(1)), cosPitch);
    plus.z = FX_Mul(FX_CosIdx(yaw + DEG_TO_IDX(1)), cosPitch);
    sinPitchPlus = FX_SinIdx(pitch + DEG_TO_IDX(1));
    if (yaw < 0x4000) {
        if (base.x < dir->x && base.z > dir->z) {
            if (MATH_ABS(base.x - dir->x) >= MATH_ABS(base.z - dir->z)) {
                yaw = yaw - (u16)((dir->x - base.x) * DEG_TO_IDX(1) / (minus.x - base.x));
            } else {
                yaw = yaw - (u16)((base.z - dir->z) * DEG_TO_IDX(1) / (base.z - minus.z));
            }
        } else if (base.x > dir->x && base.z < dir->z) {
            if (MATH_ABS(base.x - dir->x) >= MATH_ABS(base.z - dir->z)) {
                yaw = yaw + (u16)((base.x - dir->x) * DEG_TO_IDX(1) / (base.x - plus.x));
            } else {
                yaw = yaw + (u16)((dir->z - base.z) * DEG_TO_IDX(1) / (plus.z - base.z));
            }
        }
    } else if (yaw < 0x8000) {
        if (base.x > dir->x && base.z > dir->z) {
            if (MATH_ABS(base.x - dir->x) >= MATH_ABS(base.z - dir->z)) {
                yaw = yaw - (u16)((base.x - dir->x) * DEG_TO_IDX(1) / (base.x - minus.x));
            } else {
                yaw = yaw - (u16)((base.z - dir->z) * DEG_TO_IDX(1) / (base.z - minus.z));
            }
        } else if (base.x < dir->x && base.z < dir->z) {
            if (MATH_ABS(base.x - dir->x) >= MATH_ABS(base.z - dir->z)) {
                yaw = yaw + (u16)((dir->x - base.x) * DEG_TO_IDX(1) / (plus.x - base.x));
            } else {
                yaw = yaw + (u16)((dir->z - base.z) * DEG_TO_IDX(1) / (plus.z - base.z));
            }
        }
    } else if (yaw < 0xc000) {
        if (base.x > dir->x && base.z < dir->z) {
            if (MATH_ABS(base.x - dir->x) >= MATH_ABS(base.z - dir->z)) {
                yaw = yaw - (u16)((base.x - dir->x) * DEG_TO_IDX(1) / (base.x - minus.x));
            } else {
                yaw = yaw - (u16)((dir->z - base.z) * DEG_TO_IDX(1) / (minus.z - base.z));
            }
        } else if (base.x < dir->x && base.z > dir->z) {
            if (MATH_ABS(base.x - dir->x) >= MATH_ABS(base.z - dir->z)) {
                yaw = yaw + (u16)((dir->x - base.x) * DEG_TO_IDX(1) / (plus.x - base.x));
            } else {
                yaw = yaw + (u16)((base.z - dir->z) * DEG_TO_IDX(1) / (base.z - plus.z));
            }
        }
    } else {
        if (base.x < dir->x && base.z < dir->z) {
            if (MATH_ABS(base.x - dir->x) >= MATH_ABS(base.z - dir->z)) {
                yaw = yaw - (u16)((dir->x - base.x) * DEG_TO_IDX(1) / (minus.x - base.x));
            } else {
                yaw = yaw - (u16)((dir->z - base.z) * DEG_TO_IDX(1) / (minus.z - base.z));
            }
        } else if (base.x > dir->x && base.z > dir->z) {
            if (MATH_ABS(base.x - dir->x) >= MATH_ABS(base.z - dir->z)) {
                yaw = yaw + (u16)((base.x - dir->x) * DEG_TO_IDX(1) / (base.x - plus.x));
            } else {
                yaw = yaw + (u16)((base.z - dir->z) * DEG_TO_IDX(1) / (base.z - plus.z));
            }
        }
    }
    if (pitch < 0x4000) {
        if (base.y > dir->y) {
            pitch = pitch - (u16)((base.y - dir->y) * DEG_TO_IDX(1) / (base.y - sinPitchMinus));
        } else if (base.y < dir->y) {
            pitch = pitch + (u16)((dir->y - base.y) * DEG_TO_IDX(1) / (sinPitchPlus - base.y));
        }
    } else if (pitch >= 0xc000) {
        if (base.y > dir->y) {
            pitch = pitch - (u16)((base.y - dir->y) * DEG_TO_IDX(1) / (base.y - sinPitchMinus));
        } else if (base.y < dir->y) {
            pitch = pitch + (u16)((dir->y - base.y) * DEG_TO_IDX(1) / (sinPitchPlus - base.y));
        }
    }
    *outYaw = yaw;
    *outPitch = pitch;
}




G3DCamera *FieldCamera_GetG3DCamera(FieldCamera *camera) {
    return camera->g3dCamera;
}

void FieldCamera_SetBind(FieldCamera *camera, const VecFx32 *bind) {
    camera->bind = bind;
    camera->targetBase = *bind;
    camera->target = *bind;
}


void FieldCamera_ClearBind(FieldCamera *camera) {
    camera->bind = NULL;
}

void *FieldCamera_GetBind(FieldCamera *camera) {
    return (void *)camera->bind;
}

void FieldCamera_ResetBind(FieldCamera *camera) {
    camera->bind = camera->refBind;
}

void FieldCamera_SetZNear(FieldCamera *camera, fx32 zNear) {
    GFL_G3DCameraSetProjectionZNear(camera->g3dCamera, &zNear);
}

void FieldCamera_SetZFar(FieldCamera *camera, fx32 zFar) {
    GFL_G3DCameraSetProjectionZFar(camera->g3dCamera, &zFar);
}

void FieldCamera_CoordsSetTarget(FieldCamera *camera, const VecFx32 *target) {
    camera->target = *target;
    camera->targetBase = *target;
}




void FieldCamera_CoordsGetTarget(FieldCamera *camera, VecFx32 *target) {
    *target = camera->target;
}

void FieldCamera_CoordsGetTargetOffset(FieldCamera *camera, VecFx32 *offset) {
    *offset = camera->targetOffset;
}

void FieldCamera_CoordsSetTargetOffset(FieldCamera *camera, const VecFx32 *offset) {
    camera->targetOffset = *offset;
}

void FieldCamera_CoordsGetEye(FieldCamera *camera, VecFx32 *eye) {
    *eye = camera->eye;
}

void FieldCamera_CoordsSetEye(FieldCamera *camera, const VecFx32 *eye) {
    camera->eye = *eye;
}






void func_ov021_0218ed04(FieldCamera *camera, const VecFx32 *src) {
    VecFx32 *dst = &camera->eyeOffset;
    *dst = *src;
}


void func_ov021_0218ed14(FieldCamera *camera, VecFx32 *up) {
    GFL_G3DCameraSetLookatUpVector(camera->g3dCamera, up);
}

void func_ov021_0218ed20(FieldCamera *camera, VecFx32 *up) {
    GFL_G3DCameraGetLookatUpVector(camera->g3dCamera, up);
}

u16 FieldCamera_CoordsGetPitch(FieldCamera *camera) {
    return camera->pitch;
}

void FieldCamera_CoordsSetPitch(FieldCamera *camera, u16 pitch) {
    camera->pitch = pitch;
}

u16 *func_ov021_0218ed3c(FieldCamera *camera) {
    return &camera->anglePitch;
}

u16 FieldCamera_CoordsGetYaw(FieldCamera *camera) {
    return camera->yaw;
}

void FieldCamera_CoordsSetYaw(FieldCamera *camera, u16 yaw) {
    camera->yaw = yaw;
}

u16 *func_ov021_0218ed50(FieldCamera *camera) {
    return &camera->angleYaw;
}

fx32 FieldCamera_CoordsGetZoom(FieldCamera *camera) {
    return camera->zoom;
}

void FieldCamera_CoordsSetZoom(FieldCamera *camera, fx32 zoom) {
    camera->zoom = zoom;
}

void FieldCamera_CoordsDerive(VecFx32 *param_1, u16 *param_2, u16 *param_3, fx32 *param_4) {
    FieldCamera_CoordsDerive_(param_1, param_2, param_3, param_4);
}

void FieldCamera_SetFOV(FieldCamera *camera, u16 fov) {
    camera->fov = fov;
    GFL_G3DCameraPerspectiveSetFOVSin(camera->g3dCamera, FX_SinCosTable_[(camera->fov >> 4) * 2]);
    GFL_G3DCameraPerspectiveSetFOVCos(camera->g3dCamera, FX_SinCosTable_[(camera->fov >> 4) * 2 + 1]);
}

void FieldCameraDelay_Update(FieldCameraDelay *delay, VecFx32 *param_2, VecFx32 *param_3, VecFx32 *param_4, VecFx32 *param_5) {
    s32 idx;
    s32 end;
    if (delay == NULL) {
        *param_4 = *param_2;
        *param_5 = *param_3;
        return;
    }
    if (delay->active == 0) {
        *param_4 = *param_2;
        *param_5 = *param_3;
        return;
    }
    idx = delay->head;
    end = delay->tail;
    if (delay->finishing == 0) {
        delay->targets[end] = *param_2;
        delay->eyes[end] = *param_3;
        end = (end + 1) % delay->count;
    }
    if (delay->filled != 0) {
        *param_4 = delay->targets[idx];
        *param_5 = delay->eyes[idx];
    } else {
        *param_4 = delay->targets[delay->maxDelay];
        *param_5 = delay->eyes[delay->maxDelay];
        if (idx == delay->maxDelay) {
            delay->filled = 1;
        }
    }
    if (delay->finishing != 0) {
        if (idx != end) {
            idx = (idx + 1) % delay->count;
            if (idx == end) {
                FieldCameraDelay_Disable(delay);
            }
        }
    } else {
        idx = (idx + 1) % delay->count;
    }
    delay->head = idx;
    delay->tail = end;
    if (delay->delayX == 0) {
        param_4->x = param_2->x;
        param_5->x = param_3->x;
    }
    if (delay->delayY == 0) {
        param_4->y = param_2->y;
        param_5->y = param_3->y;
    }
    if (delay->delayZ == 0) {
        param_4->z = param_2->z;
        param_5->z = param_3->z;
    }
}


void func_ov021_0218eefc(s32 param_1, s32 param_2, u32 param_3, u32 heap, FieldCamera *camera) {
    s32 i;
    FieldCameraDelay *work;
    VecFx32 *a;
    VecFx32 *b;
    if (camera->bind != NULL) {
        work = GFL_HeapAllocate(heap, sizeof(FieldCameraDelay), FALSE, "field_camera.c", 0x6aa);
        work->targets = GFL_HeapAllocate(heap, param_1 * sizeof(VecFx32), FALSE, "field_camera.c", 0x6ab);
        work->eyes = GFL_HeapAllocate(heap, param_1 * sizeof(VecFx32), FALSE, "field_camera.c", 0x6ac);
        for (i = 0; i < param_1; i++) {
            a = &work->targets[i];
            a->x = 0;
            a->y = 0;
            a->z = 0;
            b = &work->eyes[i];
            b->x = 0;
            b->y = 0;
            b->z = 0;
        }
        work->count = param_1;
        work->maxDelay = param_2;
        work->filled = 0;
        work->head = 0;
        work->tail = param_2;
        work->delayX = 0;
        work->delayY = 0;
        work->delayZ = 0;
        if (param_3 & 1) {
            work->delayX = 1;
        }
        if (param_3 & 2) {
            work->delayY = 1;
        }
        if (param_3 & 4) {
            work->delayZ = 1;
        }
        work->active = 1;
        work->finishing = 0;
        camera->delay = work;
    }
}


void FieldCamera_FreeDelayManager(FieldCamera *camera) {
    if (camera->delay != NULL) {
        GFL_HeapFree(camera->delay->targets);
        GFL_HeapFree(camera->delay->eyes);
        GFL_HeapFree(camera->delay);
        camera->delay = NULL;
    }
}

void FieldCamera_ApplyDelay(FieldCamera *camera) {
    VecFx32 pos;
    VecFx32 target;
    VecFx32 outPos;
    VecFx32 outTarget;
    if (camera->bind != NULL) {
        GFL_G3DCameraGetLookatTarget(camera->g3dCamera, &target);
        GFL_G3DCameraGetLookatPos(camera->g3dCamera, &pos);
        FieldCameraDelay_Update(camera->delay, &target, &pos, &outTarget, &outPos);
        GFL_G3DCameraSetLookatTarget(camera->g3dCamera, &outTarget);
        GFL_G3DCameraSetLookatPos(camera->g3dCamera, &outPos);
    }
}


void FieldCameraDelay_Disable(FieldCameraDelay *delay) {
    delay->finishing = 0;
    delay->active = 0;
    delay->filled = 0;
}

void FieldCamera_EnableDelay(FieldCamera *camera) {
    FieldCameraDelay *delay;
    VecFx32 *v;
    s32 i;

    delay = camera->delay;
    if (delay != NULL) {
        for (i = 0; i < delay->count; i++) {
            v = &delay->targets[i];
            v->x = 0;
            v->y = 0;
            v->z = 0;
            v = &delay->eyes[i];
            v->x = 0;
            v->y = 0;
            v->z = 0;
        }
        delay->head = 0;
        delay->tail = delay->maxDelay;
        delay->filled = 0;
        delay->active = 1;
        delay->finishing = 0;
    }
}

void FieldCamera_FinishDelay(FieldCamera *camera) {
    FieldCameraDelay *delay;

    delay = camera->delay;
    if (delay != NULL) {
        if (delay->filled == 0 && delay->tail == delay->maxDelay && delay->head == 0) {
            FieldCameraDelay_Disable(delay);
            return;
        }
        delay->finishing = 1;
    }
}

// Black's code at 0x0218f0d4-0x0218fe78, once the port's ov021_0218f0d4.c: the same file (its .text follows without a
// gap and its code reads the vector constant and the boundary table above)

void func_ov021_0218f89c(FieldCamera *camera, const FieldEvCameraAnimationSetup *setup, u16 frames);
void FieldCamera_BackupCoords(FieldCamera *camera, FieldEvCameraAnimationCoords *dst);
s32 FieldCameraCalc_LerpScalar(fx32 *start, fx32 *end, s32 frames, s32 cur);
u16 FieldCameraCalc_LerpAngle(u16 *start, u16 *end, s32 frames, s32 cur);
void FieldCameraCalc_LerpVector(VecFx32 *from, VecFx32 *to, s32 frames, u16 cur, VecFx32 *out);
void FieldCameraCalc_EvCamera(FieldCamera *camera, void *arg);
void func_ov021_0218fc80(FieldCamera *camera, void *arg);
void FieldCamera_ClearAnimatorFunc(FieldCamera *camera);


#define ROUND_UNITS(u) ((s32)((u) > 0 ? (f32)((u) << 12) + 0.5f : (f32)((u) << 12) - 0.5f))


void FieldCamera_DisableDelay(FieldCamera *camera) {
    FieldCameraDelay *delay;

    delay = camera->delay;
    if (delay != NULL) {
        delay->finishing = 0;
        delay->active = 0;
        delay->filled = 0;
    }
}

BOOL FieldCamera_IsDelayActive(FieldCamera *camera) {
    if (camera->delay == NULL) {
        return 0;
    }
    return camera->delay->active;
}

BOOL FieldCamera_SupportsDelay(FieldCamera *camera) {
    if (camera->bind == NULL) {
        return 0;
    }
    if (camera->delay != NULL) {
        return 1;
    }
    return 0;
}

// A boundary file of archive 0x6c: the count, then the boundaries
void FieldCameraBoundary_LoadFile(FieldCamera *camera, s32 *file) {
    MI_CpuCopy8(file + 1, camera->boundary, *file * sizeof(FieldCameraBoundaryEntry));
    camera->boundaryCount = *file;
}

void FieldCameraBoundary_LoadDummy(FieldCamera *camera) {
    MI_CpuFill8(camera->boundary, 0, sizeof(camera->boundary));
    camera->boundaryCount = 0;
}

void FieldCameraBoundary_ChangeID(FieldCamera *camera, u16 boundaryId, HeapID heapId) {
    s32 *ptr;

    if (camera->boundaryId != boundaryId) {
        ptr = GFL_ArcToolReadHeapNewLZ(camera->boundaryArc, boundaryId, 0, heapId & 0x7fff | 0x8000);
        FieldCameraBoundary_LoadFile(camera, ptr);
        GFL_HeapFree(ptr);
        camera->boundaryId = boundaryId;
    }
}

void FieldCamera_GetTarget(FieldCamera *camera, VecFx32 *out) {
    *out = camera->targetActual;
}

void FieldCamera_GetEye(FieldCamera *camera, VecFx32 *out) {
    *out = camera->eyeActual;
}

void FieldCamera_SetUseBoundaryEnable(FieldCamera *camera, BOOL enable) {
    camera->useBoundaryEnable = enable;
}




BOOL FieldCamera_IsUseBoundaryEnable(FieldCamera *camera) {
    return camera->useBoundaryEnable;
}

// The camera's boundary functions by type (Black's .data: the pointers are not const)
BOOL (*MM_CAMBOUND_FUNCS[])(const FieldCameraBoundaryEntry *, const VecFx32 *, VecFx32 *) = {
    NULL,
    func_ov021_0218f1fc,
    func_ov021_0218f310,
    func_ov021_0218f460,
};

void FieldCameraBoundary_ApplyTarget(FieldCameraBoundaryEntry *entry, VecFx32 *param_2) {
    BOOL (*fn)(const FieldCameraBoundaryEntry *, const VecFx32 *, VecFx32 *);


    if (entry->state == 0) {
        fn = MM_CAMBOUND_FUNCS[entry->type];
        if (fn != NULL) {
            fn(entry, param_2, param_2);
        }
    }
}

void FieldCameraBoundary_ApplyEye(FieldCameraBoundaryEntry *entry, VecFx32 *param_2) {
    BOOL (*fn)(const FieldCameraBoundaryEntry *, const VecFx32 *, VecFx32 *);

    if (entry->state == 1) {
        fn = MM_CAMBOUND_FUNCS[entry->type];
        if (fn != NULL) {
            fn(entry, param_2, param_2);
        }
    }
}



BOOL func_ov021_0218f1fc(const FieldCameraBoundaryEntry *entry, const VecFx32 *in, VecFx32 *out) {
    VecFx32 pos;
    BOOL changed;

    pos = *in;
    changed = FALSE;
    out->x = pos.x;
    out->z = pos.z;
    out->y = pos.y;
    if (ROUND_UNITS(entry->unk_08) > pos.x) {
        changed = TRUE;
        out->x = ROUND_UNITS(entry->unk_08);
    } else if ((entry->unk_0C << 12) < pos.x) {
        changed = TRUE;
        out->x = ROUND_UNITS(entry->unk_0C);
    }
    if ((entry->unk_10 << 12) > pos.z) {
        changed = TRUE;
        out->z = ROUND_UNITS(entry->unk_10);
    } else if ((entry->unk_14 << 12) < pos.z) {
        changed = TRUE;
        out->z = ROUND_UNITS(entry->unk_14);
    }
    return changed;
}

BOOL func_ov021_0218f310(const FieldCameraBoundaryEntry *entry, const VecFx32 *in, VecFx32 *out) {
    VecFx32 axis = FieldCamera_AxisZ();
    VecFx32 pos = *in;
    VecFx32 orig = *in;
    VecFx32 center;
    VecFx32 offset;
    VecFx32 cross;
    fx32 dist;
    u16 angle;
    BOOL moved = FALSE;
    out->x = in->x;
    out->y = in->y;
    out->z = in->z;
    center.x = entry->unk_08;
    center.y = 0;
    pos.y = 0;
    center.z = entry->unk_0C;
    VEC_Subtract(&pos, &center, &offset);
    dist = VEC_Mag(&offset);
    if (FX_Modf(dist, &dist) > 0) {
        dist += FX32_ONE;
    }
    VEC_Normalize(&offset, &offset);
    VEC_CrossProduct(&axis, &offset, &cross);
    angle = FX_AcosIdx(VEC_DotProduct(&axis, &offset));
    if (cross.y < 0) {
        angle = 0x10000 - angle;
    }
    if (dist > entry->unk_10) {
        dist = entry->unk_10;
        moved = TRUE;
    }
    if (entry->angles[0] != entry->angles[1]) {
        if (entry->angles[0] > entry->angles[1]) {
            if (entry->angles[1] < angle && entry->angles[0] >= angle) {
                if (MATH_ABS(entry->angles[1] - angle) > MATH_ABS(entry->angles[0] - angle)) {
                    angle = entry->angles[0];
                    moved = TRUE;
                } else {
                    angle = entry->angles[1];
                    moved = TRUE;
                }
            }
        } else if (entry->angles[0] > angle) {
            angle = entry->angles[0];
            moved = TRUE;
        } else if (entry->angles[1] < angle) {
            angle = entry->angles[1];
            moved = TRUE;
        }
    }
    if (moved) {
        out->x = center.x + FX_Mul(FX_SinIdx(angle), dist);
        out->z = center.z + FX_Mul(FX_CosIdx(angle), dist);
        out->y = orig.y;
    }
    return moved;
}


BOOL func_ov021_0218f460(const FieldCameraBoundaryEntry *entry, const VecFx32 *in, VecFx32 *out) {
    VecFx32 pos;
    BOOL changed;
    s32 dist;
    s32 index;

    pos = *in;
    changed = FALSE;
    out->x = pos.x;
    out->z = pos.z;
    out->y = pos.y;
    if (ROUND_UNITS(entry->unk_08) < pos.x && ROUND_UNITS(entry->unk_0C) > pos.x && ROUND_UNITS(entry->unk_10) < pos.z && ROUND_UNITS(entry->unk_14) > pos.z) {
        changed = TRUE;
        dist = out->x - ROUND_UNITS(entry->unk_08);
        index = 0;
        if (dist > ROUND_UNITS(entry->unk_0C) - out->x) {
            dist = ROUND_UNITS(entry->unk_0C) - out->x;
            index = 1;
        }
        if (dist > out->z - ROUND_UNITS(entry->unk_10)) {
            dist = out->z - ROUND_UNITS(entry->unk_10);
            index = 2;
        }
        if (dist > ROUND_UNITS(entry->unk_14) - out->z) {
            dist = ROUND_UNITS(entry->unk_14) - out->z;
            index = 3;
        }
        switch (index) {
        case 0:
            out->x = ROUND_UNITS(entry->unk_08);
            break;
        case 1:
            out->x = ROUND_UNITS(entry->unk_0C);
            break;
        case 2:
            out->z = ROUND_UNITS(entry->unk_10);
            break;
        case 3:
            out->z = ROUND_UNITS(entry->unk_14);
            break;
        }
    }
    return changed;
}



void FieldCamera_BackupCoords(FieldCamera *camera, FieldEvCameraAnimationCoords *dst) {
    dst->cameraPos = camera->eye;
    dst->targetPos = camera->target;
    dst->pitch = camera->pitch;
    dst->yaw = camera->yaw;
    dst->distance = camera->zoom;
    dst->fov = camera->fov;
    dst->extraTranslation = camera->targetOffset;
}

void func_ov021_0218f7d0(FieldCamera *camera, FieldEvCameraAnimationCoords *dst) {
    dst->cameraPos = camera->eye;
    FieldCamera_GetTarget(camera, &dst->targetPos);
    dst->pitch = camera->pitch;
    dst->yaw = camera->yaw;
    dst->distance = camera->zoom;
    dst->fov = camera->fov;
    dst->extraTranslation = camera->targetOffset;
}

void FieldCamera_EVCameraInit(FieldCamera *camera) {
    struct FieldCameraAnm *anm;

    if (camera != NULL) {
        anm = &camera->anm;
        if (anm->initDone == 0) {
            anm->savedTransformType = camera->transformType;
            FieldCamera_ChangeTransformType(camera, 0);
            FieldCamera_BackupCoords(camera, &anm->saved1);
            anm->initDone = 1;
        }
    }
}

void FieldCameraAnm_EnsureInitDone(FieldCamera *camera) {
    struct FieldCameraAnm *anm;

    if (camera != NULL) {
        anm = &camera->anm;
        if (anm->initDone == 0) {
            anm->savedTransformType = camera->transformType;
            FieldCamera_ChangeTransformType(camera, 0);
            func_ov021_0218f7d0(camera, &anm->saved1);
            anm->initDone = 1;
        }
    }
}

void FieldCameraAnm_EVCameraEnd(FieldCamera *camera) {
    struct FieldCameraAnm *anm;

    if (camera != NULL) {
        anm = &camera->anm;
        if (anm->initDone != 0) {
            FieldCamera_ChangeTransformType(camera, anm->savedTransformType);
            anm->initDone = 0;
        }
    }
}

void func_ov021_0218f89c(FieldCamera *camera, const FieldEvCameraAnimationSetup *setup, u16 frames) {
    struct FieldCameraAnm *anm;

    if (camera != NULL) {
        anm = &camera->anm;
        FieldCamera_ChangeTransformType(camera, 0);
        anm->coords = setup->targetCoords;
        anm->frame = 0;
        anm->frames = frames;
        anm->flags = setup->flags;
        camera->anm.callback = FieldCameraCalc_EvCamera;
        anm->calcArg = NULL;
        anm->active = 1;
    }
}









void FieldCameraAnm_SetAnimation(FieldCamera *camera, const FieldEvCameraAnimationSetup *setup, u16 frames) {
    if (camera != NULL) {
        func_ov021_0218f89c(camera, setup, frames);
        FieldCamera_BackupCoords(camera, &camera->anm.saved2);
    }
}

void FieldCameraAnm_SetAnimationRealTime(FieldCamera *camera, const FieldEvCameraAnimationSetup *setup, u16 frames) {
    if (camera != NULL) {
        func_ov021_0218f89c(camera, setup, frames);
        func_ov021_0218f7d0(camera, &camera->anm.saved2);
    }
}

void func_ov021_0218f930(FieldCamera *camera, const VecFx32 *cameraPos, const VecFx32 *targetPos, const FieldEvCameraAnimationFlags *flags, u16 frames) {
    struct FieldCameraAnm *anm;
    FieldEvCameraAnimationFlags animFlags;

    if (camera != NULL) {
        anm = &camera->anm;
        FieldCamera_ChangeTransformType(camera, 2);
        FieldCamera_BackupCoords(camera, &anm->saved2);
        anm->coords.cameraPos = *cameraPos;
        anm->coords.targetPos = *targetPos;
        anm->frame = 0;
        anm->frames = frames;
        animFlags.animateTargetPos = flags->animateTargetPos;
        animFlags.animateFOV = flags->animateFOV;
        animFlags.animatePitch = FALSE;
        animFlags.animateYaw = FALSE;
        animFlags.animateTargetDistance = FALSE;
        animFlags.animateExtraTranslation = flags->animateExtraTranslation;
        anm->flags = animFlags;
        camera->anm.callback = func_ov021_0218fc80;
        anm->calcArg = NULL;
        anm->active = 1;
    }
}


void FieldCameraAnm_SetReturnAnimation(FieldCamera *camera, const FieldEvCameraAnimationFlags *flags, u16 frames) {
    struct FieldCameraAnm *anm;

    if (camera != NULL) {
        anm = &camera->anm;
        if (anm->initDone != 0) {
            if (frames != 0) {
                FieldCamera_ChangeTransformType(camera, 0);
                FieldCamera_BackupCoords(camera, &anm->saved2);
                anm->coords = anm->saved1;
                anm->frame = 0;
                anm->frames = frames;
                anm->flags = *flags;
                camera->anm.callback = FieldCameraCalc_EvCamera;
                anm->calcArg = NULL;
                anm->active = 1;
            }
        }
    }
}

void FieldCameraAnm_SetLoadDefaultsAnimation(FieldCamera *camera, u16 frames) {
    struct FieldCameraAnm *anm;
    struct FieldCameraDefaults defaults;
    const VecFx32 *target;

    if (camera != NULL) {
        anm = &camera->anm;
        if (anm->initDone != 0) {
            if (frames != 0) {
                FieldCamera_ChangeTransformType(camera, 0);
                FieldCamera_BackupCoords(camera, &anm->saved2);
                FieldCamera_ReadDefaults(camera, &defaults);
                anm->coords.pitch = defaults.pitch;
                anm->coords.yaw = defaults.yaw;
                anm->coords.distance = defaults.distance << 12;
                anm->coords.fov = defaults.fov;
                if (defaults.bind != 0) {
                    anm->coords.extraTranslation = defaults.target;
                    target = camera->refBind;
                } else {
                    anm->coords.extraTranslation.y = 0;
                    anm->coords.extraTranslation.x = 0;
                    anm->coords.extraTranslation.z = 0;
                    target = &defaults.target;
                }
                anm->coords.targetPos = *target;
                anm->frame = 0;
                anm->frames = frames;
                anm->flags.animateExtraTranslation = TRUE;
                anm->flags.animatePitch = TRUE;
                anm->flags.animateYaw = TRUE;
                anm->flags.animateTargetDistance = TRUE;
                anm->flags.animateFOV = TRUE;
                anm->flags.animateTargetPos = TRUE;
                camera->anm.callback = FieldCameraCalc_EvCamera;
                anm->calcArg = NULL;
                anm->active = 1;
            }
        }
    }
}




void FieldCameraCalc_EvCamera(FieldCamera *camera, void *arg) {
    struct FieldCameraAnm *anm;
    FieldEvCameraAnimationCoords *start;
    FieldEvCameraAnimationCoords *end;

    anm = &camera->anm;
    if (anm->active != 0) {
        anm->frame++;
        start = &anm->saved2;
        end = &anm->coords;
        if (anm->flags.animatePitch) {
            camera->pitch = FieldCameraCalc_LerpAngle(&start->pitch, &end->pitch, anm->frames, anm->frame);
        }
        if (anm->flags.animateYaw) {
            camera->yaw = FieldCameraCalc_LerpAngle(&start->yaw, &end->yaw, anm->frames, anm->frame);
        }
        if (anm->flags.animateTargetPos) {
            FieldCameraCalc_LerpVector(&start->targetPos, &end->targetPos, anm->frames, anm->frame, &camera->target);
        }
        if (anm->flags.animateFOV) {
            camera->fov = FieldCameraCalc_LerpAngle(&start->fov, &end->fov, anm->frames, anm->frame);
            GFL_G3DCameraPerspectiveSetFOVSin(camera->g3dCamera, FX_SinIdx(camera->fov));
            GFL_G3DCameraPerspectiveSetFOVCos(camera->g3dCamera, FX_CosIdx(camera->fov));
        }
        if (anm->flags.animateTargetDistance) {
            camera->zoom = FieldCameraCalc_LerpScalar(&start->distance, &end->distance, anm->frames, anm->frame);
        }
        if (anm->flags.animateExtraTranslation) {
            FieldCameraCalc_LerpVector(&start->extraTranslation, &end->extraTranslation, anm->frames, anm->frame, &camera->targetOffset);
        }
        if (anm->frame >= anm->frames) {
            anm->active = 0;
            FieldCamera_ClearAnimatorFunc(camera);
        }
    }
}

void func_ov021_0218fc80(FieldCamera *camera, void *arg) {
    struct FieldCameraAnm *anm;
    FieldEvCameraAnimationCoords *start;
    FieldEvCameraAnimationCoords *end;

    anm = &camera->anm;
    if (anm->active != 0) {
        anm->frame++;
        start = &anm->saved2;
        end = &anm->coords;
        if (anm->flags.animateTargetPos) {
            FieldCameraCalc_LerpVector(&start->targetPos, &end->targetPos, anm->frames, anm->frame, &camera->target);
            FieldCameraCalc_LerpVector(&start->cameraPos, &end->cameraPos, anm->frames, anm->frame, &camera->eye);
        }
        if (anm->flags.animateFOV) {
            camera->fov = FieldCameraCalc_LerpAngle(&start->fov, &end->fov, anm->frames, anm->frame);
            GFL_G3DCameraPerspectiveSetFOVSin(camera->g3dCamera, FX_SinIdx(camera->fov));
            GFL_G3DCameraPerspectiveSetFOVCos(camera->g3dCamera, FX_CosIdx(camera->fov));
        }
        if (anm->flags.animateExtraTranslation) {
            FieldCameraCalc_LerpVector(&start->extraTranslation, &end->extraTranslation, anm->frames, anm->frame, &camera->targetOffset);
        }
        if (anm->frame >= anm->frames) {
            anm->active = 0;
            FieldCamera_ClearAnimatorFunc(camera);
        }
    }
}



void FieldCameraCalc_LerpVector(VecFx32 *from, VecFx32 *to, s32 frames, u16 cur, VecFx32 *out) {
    if (frames == 0) {
        *out = *to;
    }
    out->x = FieldCameraCalc_LerpScalar(&from->x, &to->x, frames, cur);
    out->y = FieldCameraCalc_LerpScalar(&from->y, &to->y, frames, cur);
    out->z = FieldCameraCalc_LerpScalar(&from->z, &to->z, frames, cur);
}


s32 FieldCameraCalc_LerpScalar(fx32 *start, fx32 *end, s32 frames, s32 cur) {
    fx32 base;
    fx32 last;
    fx32 delta;

    base = *start;
    last = *end;
    if (frames == cur) {
        return last;
    }
    delta = last - base;
    return base + delta * cur / frames;
}

u16 FieldCameraCalc_LerpAngle(u16 *start, u16 *end, s32 frames, s32 cur) {
    u16 base;
    u16 diff;
    u16 other;

    if (frames == 0) {
        return *end;
    }
    base = *start;
    diff = *end - base;
    other = (u16)(0x10000 - diff);
    if (diff <= other) {
        return (u16)(base + (u16)(diff * cur / frames));
    }
    return (u16)(base - (u16)(other * cur / frames));
}



void FieldCamera_ClearAnimatorFunc(FieldCamera *camera) {
    camera->anm.callback = NULL;
    camera->anm.calcArg = NULL;
}

BOOL FieldCamera_IsAnimating(FieldCamera *camera) {
    struct FieldCameraAnm *anm;

    anm = &camera->anm;
    if (camera->anm.callback != NULL) {
        if (anm->active != 0) {
            return TRUE;
        }
    }
    return FALSE;
}


FieldEvCameraAnimationCoords *func_ov021_0218fe70(FieldCamera *camera) {
    return &camera->anm.coords;
}
