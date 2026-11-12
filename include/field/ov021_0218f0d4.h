#ifndef BWPORT_FIELD_OV021_0218F0D4_H
#define BWPORT_FIELD_OV021_0218F0D4_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "field/field_camera.h"

// Defined in src/ov021/ov021_0218f0d4.c. Each takes one 0x18-byte entry of a camera's array and three vectors.
void FieldCameraBoundary_ApplyTarget(FieldCameraBoundaryEntry *param_1, VecFx32 *param_2);
void FieldCameraBoundary_ApplyEye(FieldCameraBoundaryEntry *param_1, VecFx32 *param_2);

#endif
