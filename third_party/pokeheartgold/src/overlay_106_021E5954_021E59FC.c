#include "global.h"

#include "camera.h"
#include "overlay_106.h"
#include "overlay_106_legendary_cinematic.h"

void ov106_021E59FC(LegendaryCinematic *cinematic, const LegendaryCinematicCamera *params) {
    VecFx32 *temp_r3;

    temp_r3 = &cinematic->cameraTarget;
    cinematic->cameraTarget = params->target;
    Camera_Init_FromTargetDistanceAndAngle(temp_r3, params->distance, &params->angle, params->perspectiveAngle, params->perspectiveType, 1, cinematic->camera);
    Camera_SetPerspectiveClippingPlane(params->nearClip, params->farClip, cinematic->camera);
    Camera_SetStaticPtr(cinematic->camera);
}
