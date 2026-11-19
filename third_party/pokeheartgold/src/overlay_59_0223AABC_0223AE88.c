#include "global.h"
#include "overlay_59_juice_feed_app.h"

void ov59_0223AE88(Ov59JuiceFeedApp *arg0);

void ov59_0223AE88(Ov59JuiceFeedApp *arg0) {
    s32 var_r4;

    var_r4 = 0;
    if ((s32) arg0->monCount > 0) {
        do {
            ov59_0223BADC(&arg0->mons[var_r4]);
            var_r4 += 1;
        } while (var_r4 < (s32) arg0->monCount);
    }
}
