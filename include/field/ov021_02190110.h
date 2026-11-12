#ifndef FIELD_OV021_02190110_H
#define FIELD_OV021_02190110_H

#include "types.h"
#include "struct_decls.h"
#include "field/field_camera.h"

FieldCamera *FieldCameraAnimationEx_GetFieldCamera(FieldCameraAnimationController *controller);
const FieldCameraAnimation *FieldCameraAnimationController_GetAnimation(FieldCameraAnimationController *controller);

#endif // FIELD_OV021_02190110_H
