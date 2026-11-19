#include "global.h"
#include "overlay_59_juice_feed_app.h"

s32 ov59_0223AC48(Ov59JuiceFeedApp *arg0) {
    switch (arg0->state) {
    case 0:
        if (arg0->statIndex < PERFORMANCE_MAX) {
            do {
                if (arg0->deltas[arg0->statIndex] != 0) {
                    break;
                }
                arg0->statIndex++;
            } while (arg0->statIndex < PERFORMANCE_MAX);
        }
        arg0->state++;
    case 1:
        if (ov59_0223B164(arg0) != 0) {
            arg0->state = 0;
            arg0->statIndex++;
            arg0->deltaCount--;
            if (arg0->deltaCount == 0) {
                return 1;
            }
        }
        break;
    }
    return 5;
}
