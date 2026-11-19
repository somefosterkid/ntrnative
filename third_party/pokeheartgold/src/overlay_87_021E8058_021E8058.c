#include "global.h"

#include "sprite.h"
#include "overlay_87_scratch_off_card_app.h"

#include "sprite.h"

extern VecFx32 ov87_021E83A8;

void ov87_021E8058(ScratchOffCardsAppSprite *appSprite, u32 unused);

void ov87_021E8058(ScratchOffCardsAppSprite *appSprite, u32 unused) {
    Sprite_SetScaleAndAffineType(appSprite->sprite, &ov87_021E83A8, 1U);
}
