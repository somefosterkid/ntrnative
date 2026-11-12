// Black's code at ov021 0x02190110-0x021901ec. No name is known: the module and address.

#include "types.h"
#include "field/ov021_021c6ea8.h"
#include "field/field_camera.h"
#include "field/ov021_02190110.h"
#include "field/field_task.h"
#include "field/field.h"


void FieldCameraAnimationController_CommitDstCoords(FieldCameraAnimationController *controller) {
    FieldCamera *camera = controller->camera;
    const FieldCameraAnimation *animation = FieldCameraAnimationController_GetAnimation(controller);
    const FieldCameraCoords *dst = &animation->dst;

    FieldCamera_CoordsSetPitch(camera, dst->pitch);
    FieldCamera_CoordsSetYaw(camera, dst->yaw);
    FieldCamera_CoordsSetZoom(camera, dst->distance);
    FieldCamera_CoordsSetTarget(camera, &dst->target);
    FieldCamera_CoordsSetTargetOffset(camera, &dst->offset);
}

void FieldCameraAnimationController_SetupTasks(FieldCameraAnimationController *controller) {
    Field *field = controller->field;
    const FieldCameraAnimation *animation = FieldCameraAnimationController_GetAnimation(controller);
    const FieldCameraCoords *src = &animation->src;
    const FieldCameraCoords *dst = &animation->dst;
    u32 duration = animation->frames;
    FieldTask *pitchTask = FieldCameraMoveTaskPitch_Callback(field, duration, dst->pitch);
    FieldTask *yawTask = FieldCameraMoveTaskYaw_Callback(field, duration, dst->yaw);
    FieldTask *zoomTask = FieldCameraMoveTaskZoom_Create(field, duration, dst->distance);
    FieldTask *targetTask = (FieldTask *)FieldCameraMoveTaskTarget_Create(field, duration, &src->target, &dst->target);
    FieldTask *offsetTask = FieldCameraMoveTaskTargetOffs_Create(field, duration, &dst->offset);
    FieldTaskManager *mgr = Field_GetTaskManager(field);

    FieldTaskManager_AddTask(mgr, pitchTask, 0);
    FieldTaskManager_AddTask(mgr, yawTask, 0);
    FieldTaskManager_AddTask(mgr, zoomTask, 0);
    FieldTaskManager_AddTask(mgr, targetTask, 0);
    FieldTaskManager_AddTask(mgr, offsetTask, 0);
}


FieldCamera *FieldCameraAnimationEx_GetFieldCamera(FieldCameraAnimationController *controller) {
    return controller->camera;
}

const FieldCameraAnimation *FieldCameraAnimationController_GetAnimation(FieldCameraAnimationController *controller) {
    return controller->animation;
}
