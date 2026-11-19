#include "global.h"

#include "frontier/overlay_80_02238034.h"

u32 ov80_02238430(ArcadeContext *ctx, u8 a1) {
    ov80_02238498(ctx);
    if (a1 == 0x1C) {
        return 1U;
    }
    return 3U;
}
