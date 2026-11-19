#include "global.h"

#include "math_util.h"
#include "sprite.h"
#include "overlay_02_hmcut_in.h"

struct HMCutIn;

struct Sprite;

enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};

Sprite *ov02_0224A33C(HMCutIn *cutIn, const VecFx32 *position, u32 charResourceID, u32 plttResourceID, u32 cellResourceID, u32 animResourceID, int headerPriority, int listPriority);

Sprite *ov02_0224A468(HMCutIn *cutIn, const VecFx32 *position, int priority, int dummy);

Sprite *ov02_0224A468(HMCutIn *cutIn, const VecFx32 *position, int priority, int dummy) {
    Sprite *birdSprite;
    VecFx32 translation = { 0, 0, 0 };
    VecFx32 scale = { 0x1000, 0x1000, 0 };

    birdSprite = ov02_0224A33C(cutIn, position, 0, 0, 0, -1, 0, priority);

    Sprite_SetAffineOverwriteMode(birdSprite, AFFINE_OVERWRITE_MODE_DOUBLE);
    Sprite_SetAffineMatrix(birdSprite, &translation);
    Sprite_SetAffineScale(birdSprite, &scale);
    Sprite_SetAffineZRotation(birdSprite, GF_DegreeToSinCosIdx(0));

    return birdSprite;
}
