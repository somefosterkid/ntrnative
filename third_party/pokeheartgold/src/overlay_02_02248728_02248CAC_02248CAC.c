#include "global.h"

#include "math_util.h"
#include "sprite.h"
#include "overlay_02_fly_landing.h"

struct FlyResources;

struct Sprite;

enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};

Sprite *ov02_02248C10(FlyResources *flyResources, const VecFx32 *position, u32 charResourceID, u32 paletteResourceID, u32 cellResourceID, u32 animResourceID, int headerPriority, int listPriority);

Sprite *ov02_02248CAC(FlyResources *param0);

Sprite *ov02_02248CAC(FlyResources *param0) {
    Sprite *sprite;
    VecFx32 coordinateVector = { 0, 0, 0 };
    VecFx32 translationVector = { 0, 0, 0 };
    VecFx32 scalingVector = { 0x1000, 0x1000, 0 };

    sprite = ov02_02248C10(param0, &coordinateVector, 0, 0, 0, 2, 0, 132);

    Sprite_SetAffineOverwriteMode(sprite, AFFINE_OVERWRITE_MODE_DOUBLE);
    Sprite_SetAffineMatrix(sprite, &translationVector);
    Sprite_SetAffineScale(sprite, &scalingVector);
    Sprite_SetAffineZRotation(sprite, GF_DegreeToSinCosIdx(0));

    return sprite;
}
