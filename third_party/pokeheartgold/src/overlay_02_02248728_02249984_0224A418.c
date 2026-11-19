#include "global.h"

#include "overlay_02_hmcut_in.h"
#include "sprite.h"

Sprite *ov02_0224A33C(HMCutIn *cutIn, const VecFx32 *position, u32 charResourceID, u32 paletteResourceID, u32 cellResourceID, u32 animResourceID, int headerPriority, int listPriority);

Sprite *ov02_0224A418(HMCutIn *cutIn, const VecFx32 *position);

Sprite *ov02_0224A418(HMCutIn *cutIn, const VecFx32 *position) {
    Sprite *sprite;
    s32 v0;

    v0 = 0;
    if (cutIn->playerSpriteSet != 0) {
        v0 = 1;
    }
    sprite = ov02_0224A33C(cutIn, position, 2, v0, 2, 1, 0, 0x83);
    Sprite_SetDrawFlag(sprite, 0);
    Sprite_SetAnimCtrlSeq(sprite, 6);
    return sprite;
}

void ov02_0224A450(Sprite *arg0);

void ov02_0224A450(Sprite *sprite) {
    Sprite_SetAnimActiveFlag(sprite, 1);
    Sprite_SetAnimSpeed(sprite, 0x1000);
}
