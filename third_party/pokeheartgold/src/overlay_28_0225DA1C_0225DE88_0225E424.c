#include "global.h"

#include "math_util.h"
#include "overlay_28.h"
#include "sprite.h"

void ov28_0225E424(Sprite *sprite, u32 deg) {
    Sprite_SetRotationAndAffineType(sprite, (u32)GF_DegreeToSinCosIdx(deg), 2U);
}
