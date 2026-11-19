#include "global.h"

#include "overlay_120.h"

void ov120_0225F6AC(Ov120Wobble *wobble, s16 base, s32 end, s16 speed) {
    wobble->value = base;
    wobble->base = base;
    wobble->amplitude = (s16)(end - base);
    wobble->speed = speed;
    wobble->angle = 0;
}
