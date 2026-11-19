#ifndef POKEHEARTGOLD_FIELD_CAMERA_TEMPLATE_H
#define POKEHEARTGOLD_FIELD_CAMERA_TEMPLATE_H

#include "global.h"
#include "camera.h"

// One row (0x24 bytes) of the field camera table ov01_02206478, indexed by camera type.
typedef struct FieldCameraTemplate {
    fx32 distance;
    CameraAngle angle;
    u16 perspectiveType;
    u16 perspectiveAngle;
    fx32 nearClip;
    fx32 farClip;
    VecFx32 lookAtOffset;
} FieldCameraTemplate;

extern FieldCameraTemplate ov01_02206478[];

#endif // POKEHEARTGOLD_FIELD_CAMERA_TEMPLATE_H
