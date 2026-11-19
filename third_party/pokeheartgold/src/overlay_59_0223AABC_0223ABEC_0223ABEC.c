#include "global.h"
#include "overlay_59_juice_feed_app.h"

s32 ov59_0223B11C();

s32 ov59_0223ABEC(Ov59JuiceFeedApp *arg0);

s32 ov59_0223ABEC(Ov59JuiceFeedApp *arg0) {
    u16 temp_r1;

    temp_r1 = arg0->state;
    switch (temp_r1) {
    case 0:
        if (ov59_0223AF9C(arg0) != 0) {
            if (arg0->deltaCount != 0) {
                arg0->state = 0U;
                arg0->statIndex = 0;
                return 5;
            }
            arg0->state = (u16)(arg0->state + 1);
            break;
        }
    default:
        break;
    case 1:
        if (ov59_0223B11C() != 0) {
            arg0->state = 0U;
            return 1;
        }
        break;
    }
    return 4;
}
