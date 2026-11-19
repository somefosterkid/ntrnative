#include "global.h"

#include "sprite.h"

Sprite *ov96_021E8BAC(void *param_1);

void ov96_021E9204(void *param_1, f32 x, f32 y);

void ov96_021E9204(void *param_1, f32 x, f32 y) {
    VecFx32 scale = { 0, 0, 0 };
    Sprite *sprite = ov96_021E8BAC(param_1);
    scale.x = FX_F32_TO_FX32(x);
    scale.y = FX_F32_TO_FX32(y);
    Sprite_SetAffineScale(sprite, &scale);
}
