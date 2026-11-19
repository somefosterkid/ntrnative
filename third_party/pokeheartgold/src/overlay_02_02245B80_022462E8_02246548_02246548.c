#include "global.h"

#include "camera.h"
#include "field_system_def.h"
#include "overlay_02_02245B80.h"

void ov02_02246548(LegendCameraShake *shake, s32 param1, s32 param2, u16 param3, u16 param4) {
    VecFx32 v0;

    shake->amplitudeX = FX32_CONST(param1);
    shake->amplitudeY = FX32_CONST(param2);
    shake->repeat = param3;
    shake->frames = param4;
    shake->frame = 0;
    shake->angleStep = FX_Div(360 << FX32_SHIFT, FX32_CONST(shake->frames));
    Camera_Copy(shake->fieldSystem->camera, shake->camera);
    v0 = Camera_GetLookAtCamTarget(shake->camera);
    shake->target = v0;
    shake->origin = shake->target;
    Camera_SetFixedTarget(&shake->target, shake->camera);
    Camera_SetStaticPtr(shake->camera);
}
