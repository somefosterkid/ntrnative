// Black's code at ov021 0x0218fe78-0x02190110. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field.h"
#include "field/field_camera.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_vec.h"
#include "struct_decls.h"
#include "field/field_nogrid_mapper.h"
#include "field/field_scene_area.h"
#include "field/ov021_02190110.h"

void FieldCameraAnimationController_AdjustTransform(FieldCameraAnimationController *controller);
void FieldCamera_CoordsRestoreFromLookAt(FieldCamera *camera);
void FieldCamera_CoordsRestoreZoom(FieldCamera *camera);
void FieldCameraAnimationController_SetupCameraBind(FieldCameraAnimationController *controller);
void FieldCameraAnimationController_CommitDstCoords(FieldCameraAnimationController *controller);
void FieldCameraAnimationController_SetupTasks(FieldCameraAnimationController *controller);
void func_ov021_02190000(FieldCameraAnimationController *controller);

void *Field_GetSceneArea(Field *field);
void func_ov021_021900c4(FieldCamera *camera, FieldCameraCoords *coords);
void func_ov021_0218ff48(FieldCameraAnimationController *controller);
FieldCameraAnimationController *FieldCameraAnimationController_Create(HeapID param_1);
void FieldCameraAnimationController_SetField(FieldCameraAnimationController *controller, Field *field);


void FieldCamera_GetAnimationCoords(FieldCamera *camera, FieldCameraCoords *coords) {
    func_ov021_021900c4(camera, coords);
}

FieldCameraAnimationController *Field_CreateCameraAnimationController(Field *field) {
    FieldCameraAnimationController *controller = FieldCameraAnimationController_Create(Field_GetHeapID(field));
    FieldCameraAnimationController_SetField(controller, field);
    return controller;
}


void FieldCameraAnimationController_FreeAnimation(FieldCameraAnimationController *controller) {
    func_ov021_0218ff48(controller);
}

void FieldCameraAnimationController_SetAnimation(FieldCameraAnimationController *controller,
    const FieldCameraAnimation *animation) {
    controller->animation = animation;
}



void FieldCameraAnimationController_PrepareCamera(FieldCameraAnimationController *controller) {
    FieldCameraAnimationController_AdjustTransform(controller);
    FieldCamera_CoordsRestoreFromLookAt(controller->camera);
    FieldCamera_CoordsRestoreZoom(controller->camera);
    FieldCameraAnimationController_SetupCameraBind(controller);
}

void FieldCameraAnimationController_CommitSrcCoords(FieldCameraAnimationController *controller) {
    FieldCamera *camera = FieldCameraAnimationEx_GetFieldCamera(controller);
    const FieldCameraAnimation *animation = FieldCameraAnimationController_GetAnimation(controller);
    const FieldCameraCoords *src = &animation->src;

    FieldCamera_CoordsSetPitch(camera, src->pitch);
    FieldCamera_CoordsSetYaw(camera, src->yaw);
    FieldCamera_CoordsSetZoom(camera, src->distance);
    FieldCamera_CoordsSetTarget(camera, &src->target);
    FieldCamera_CoordsSetTargetOffset(camera, &src->offset);
}


void FieldCameraAnimationController_StartAnimation(FieldCameraAnimationController *controller) {
    const FieldCameraAnimation *animation;

    animation = FieldCameraAnimationController_GetAnimation(controller);
    if (animation->frames == 0) {
        FieldCameraAnimationController_CommitDstCoords(controller);
        return;
    }
    FieldCameraAnimationController_SetupTasks(controller);
}

void FieldCameraAnimationController_FinishCamera(FieldCameraAnimationController *controller) {
    if (controller->started != 0) {
        func_ov021_02190000(controller);
    }
}

FieldCameraAnimationController *FieldCameraAnimationController_Create(HeapID param_1) {
    return GFL_HeapAllocate(param_1, sizeof(FieldCameraAnimationController), 0, "field_camera_anime.c", 0xee);
}

void func_ov021_0218ff48(FieldCameraAnimationController *controller) {
    GFL_HeapFree(controller);
}



void FieldCameraAnimationController_SetField(FieldCameraAnimationController *controller, Field *field) {
    FieldCamera *camera;

    MI_CpuFill8(controller, 0, sizeof(FieldCameraAnimationController));
    controller->field = field;
    camera = Field_GetCameraSystem(field);
    controller->camera = camera;
    controller->animation = NULL;
    controller->started = 0;
    controller->useBoundaryEnable = 0;
    controller->bind = NULL;
    controller->hasBind = 0;
}

void FieldCameraAnimationController_SetupCameraBind(FieldCameraAnimationController *controller) {
    Field *field;
    FieldCamera *camera;
    const FieldCameraAnimation *animation;
    u32 useBoundaryEnable;
    NoGridMapper *mapper;
    void *bind;
    FieldSceneArea *area;


    field = controller->field;
    camera = controller->camera;
    animation = FieldCameraAnimationController_GetAnimation(controller);
    useBoundaryEnable = Field_GetResolvedControllerTypeID(field);
    if (useBoundaryEnable == 1) {
        mapper = Field_GetNoGridMapper(field);
        FieldNoGridMapper_SetCameraAreaEnabled(mapper, 0);
    }
    if ((animation->unk48 != 0) &&
            (bind = FieldCamera_GetBind(controller->camera), bind != (void *)0x0)) {
        controller->hasBind = 1;
        bind = FieldCamera_GetBind(camera);
        controller->bind = bind;
        FieldCamera_ClearBind(camera);
    }
    if (animation->unk4C != 0) {
        useBoundaryEnable = FieldCamera_IsUseBoundaryEnable(camera);
        controller->useBoundaryEnable = useBoundaryEnable;
        FieldCamera_SetUseBoundaryEnable(camera, 0);
    }
    area = Field_GetSceneArea(field);
    SetFieldSceneAreaCameraAreaEnable(area, 0);
    controller->started = 1;
}

void FieldCameraAnimationController_AdjustTransform(FieldCameraAnimationController *controller) {
    FieldCamera *camera;

    camera = controller->camera;
    controller->transformType = FieldCamera_GetTransformType(camera);
    FieldCamera_ChangeTransformType(camera, 0);
}

void func_ov021_02190000(FieldCameraAnimationController *controller) {
    Field *field;
    FieldCamera *camera;
    const FieldCameraAnimation *animation;
    FieldSceneArea *area;
    NoGridMapper *mapper;


    field = controller->field;
    camera = controller->camera;
    animation = FieldCameraAnimationController_GetAnimation(controller);
    area = Field_GetSceneArea(field);
    SetFieldSceneAreaCameraAreaEnable(area, 1);
    if (animation->unk4C != 0) {
        FieldCamera_SetUseBoundaryEnable(camera, controller->useBoundaryEnable);
    }
    if (controller->hasBind != 0) {
        FieldCamera_SetBind(camera, controller->bind);
    }
    FieldCamera_ChangeTransformType(camera, controller->transformType);
    if (Field_GetResolvedControllerTypeID(field) == 1) {
        mapper = Field_GetNoGridMapper(field);
        FieldNoGridMapper_SetCameraAreaEnabled(mapper, 1);
    }
}

void FieldCamera_CoordsRestoreFromLookAt(FieldCamera *camera) {
    VecFx32 target;
    VecFx32 eye;
    VecFx32 dir;
    fx32 zoom;
    u16 pitch;
    u16 yaw;


    FieldCamera_GetEye(camera, &eye);
    FieldCamera_GetTarget(camera, &target);
    VEC_Subtract(&eye, &target, &dir);
    FieldCamera_CoordsDerive(&dir, &pitch, &yaw, &zoom);
    FieldCamera_CoordsSetZoom(camera, zoom);
    FieldCamera_CoordsSetPitch(camera, pitch);
    FieldCamera_CoordsSetYaw(camera, yaw);
}

void FieldCamera_CoordsRestoreZoom(FieldCamera *camera) {
    VecFx32 target;

    FieldCamera_GetTarget(camera, &target);
    FieldCamera_CoordsSetTarget(camera, &target);
}

void func_ov021_021900c4(FieldCamera *camera, FieldCameraCoords *coords) {
    VecFx32 target;
    VecFx32 eye;
    VecFx32 dir;


    FieldCamera_GetEye(camera, &eye);
    FieldCamera_GetTarget(camera, &target);
    VEC_Subtract(&eye, &target, &dir);
    FieldCamera_CoordsDerive(&dir, &coords->pitch, &coords->yaw, &coords->distance);
    FieldCamera_GetTarget(camera, &coords->target);
    FieldCamera_CoordsGetTargetOffset(camera, &coords->offset);
}
