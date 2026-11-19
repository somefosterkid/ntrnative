#include "global.h"

#include "sprite.h"
#include "touchscreen.h"

void ov70_022410F0(Sprite *sprite, int x, int y);

extern TouchscreenHitbox ov70_02245D26;
s32 ov70_02241164(s32 arg0);

void ov70_022410F0(Sprite *sprite, int x, int y) {
    VecFx32 pos;

    pos.x = ((fx32)(((x) > 0) ? ((x) * (1 << 12) + 0.5f) : ((x) * (1 << 12) - 0.5f)));
    pos.y = ((fx32)(((y - 8) > 0) ? ((y - 8) * (1 << 12) + 0.5f) : ((y - 8) * (1 << 12) - 0.5f))) + (256 * ((fx32)0x0000000000001000L));
    pos.z = 0;

    Sprite_SetMatrix(sprite, &pos);
}

s32 ov70_02241164(s32 arg0) {
    s32 v0;

    v0 = TouchscreenHitbox_FindRectAtTouchNew(&ov70_02245D26);
    if ((v0 == -1) || (v0 >= arg0)) {
        v0 = -1;
    }
    return v0;
}
