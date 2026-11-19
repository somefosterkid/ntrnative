#include "global.h"
#include "overlay_87_scratch_off_card_app.h"

struct CoordinatesS16;
struct Sprite;
enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};
typedef struct Sprite Sprite;
void Sprite_SetScaleAndAffineType(Sprite *sprite, const VecFx32 *scale, u8 mode);
void Sprite_SetPalIndexRespectVramOffset(Sprite *sprite, BOOL draw);
void Sprite_SetAnimActiveFlag(Sprite *sprite, BOOL animate);
void Sprite_SetAnimSpeed(Sprite *sprite, fx32 speed);
void Sprite_SetAnimCtrlSeq(Sprite *sprite, u32 animID);
u16 Sprite_GetAnimationNumber(const Sprite *sprite);
void Sprite_SetMosaic(Sprite *sprite, BOOL draw);
void Sprite_IsAnimated(Sprite *sprite, BOOL draw);

void ov87_021E806C(ScratchOffCardsAppSprite *appSprite, VecFx32 *scale);
void ov87_021E8078(ScratchOffCardsAppSprite *appSprite, BOOL draw);
void ov87_021E8084(ScratchOffCardsAppSprite *sprite, BOOL animate);
void ov87_021E80A8(ScratchOffCardsAppSprite *appSprite, BOOL draw);
void ov87_021E80B4(ScratchOffCardsAppSprite *appSprite, BOOL draw);

void ov87_021E806C(ScratchOffCardsAppSprite *appSprite, VecFx32 *scale) {
    Sprite_SetScaleAndAffineType(appSprite->sprite, scale, AFFINE_OVERWRITE_MODE_DOUBLE);
}

void ov87_021E8078(ScratchOffCardsAppSprite *appSprite, BOOL draw) {
    Sprite_SetPalIndexRespectVramOffset(appSprite->sprite, draw);
}

void ov87_021E8084(ScratchOffCardsAppSprite *sprite, BOOL animate) {
    Sprite_SetAnimActiveFlag(sprite->sprite, animate);
    Sprite_SetAnimSpeed(sprite->sprite, ((fx32)0x0000000000001000L));
    Sprite_SetAnimCtrlSeq(sprite->sprite, Sprite_GetAnimationNumber(sprite->sprite));
}

void ov87_021E80A8(ScratchOffCardsAppSprite *appSprite, BOOL draw) {
    Sprite_SetMosaic(appSprite->sprite, draw);
}

void ov87_021E80B4(ScratchOffCardsAppSprite *appSprite, BOOL draw) {
    Sprite_IsAnimated(appSprite->sprite, draw);
}
