#include "global.h"

#include "camera.h"
#include "field_system_def.h"
#include "overlay_02_02245B80.h"

void ov02_0224662C(LegendCameraShake *shake) {
    shake->frame = 0;
    Camera_SetStaticPtr(shake->fieldSystem->camera);
}
