#include "global.h"

#include "sprite.h"
#include "overlay_02_hmcut_in.h"

struct HMCutIn;

Sprite *ov02_0224A33C(HMCutIn *cutIn, const VecFx32 *position, u32 charResourceID, u32 plttResourceID, u32 cellResourceID, u32 animResourceID, int headerPriority, int listPriority);

Sprite *ov02_0224A3F0(HMCutIn *cutIn, const VecFx32 *position, int listPriority, int animID);

Sprite *ov02_0224A3F0(HMCutIn *cutIn, const VecFx32 *position, int listPriority, int animID) {
    Sprite *sprite = ov02_0224A33C(cutIn, position, 1, 0, 1, 0, 0, listPriority);
    Sprite_SetAnimCtrlSeq(sprite, animID);

    return sprite;
}
