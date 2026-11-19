#include "global.h"

#include "overlay_120.h"

void ov120_0225F6BC(Ov120Wobble *wobble) {
    wobble->value = (s16)(wobble->base + (((s32)(FX_SinCosTable_[(((s32)wobble->angle) >> 4) * 2] * wobble->amplitude)) >> 0xC));
    wobble->angle = (u16)(wobble->angle + (((s32)(13107 * (wobble->speed * 5))) / 360));
}
