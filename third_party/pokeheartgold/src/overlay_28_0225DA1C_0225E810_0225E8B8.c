#include "global.h"
#include "overlay_28.h"
#include "sprite.h"
#include "touchscreen.h"

s32 ov28_0225E8B8(const TouchscreenHitbox *hitbox, Sprite *sprite, s32 arg2, s32 seq) {
    s32 v0;
    s32 v1;
    u16 v2;

    v0 = arg2;
    v1 = TouchscreenHitbox_TouchHeldIsIn(hitbox);
    v2 = Sprite_GetAnimationNumber(sprite);
    if ((v1 == 1) && (System_GetTouchNew() != 0)) {
        Sprite_SetAnimCtrlSeq(sprite, seq);
        return 1;
    }
    if ((v1 == 1) && (v2 == seq)) {
        return 1;
    }
    Sprite_SetAnimCtrlSeq(sprite, v0);
    return 0;
}
