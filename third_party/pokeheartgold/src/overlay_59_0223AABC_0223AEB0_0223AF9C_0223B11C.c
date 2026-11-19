#include "global.h"
#include "overlay_59_juice_feed_app.h"

#include "text.h"

void ov59_0223BBB0(Ov59JuiceFeedApp *param_1);

s32 ov59_0223B11C(Ov59JuiceFeedApp *arg0, s32 arg1, s32 arg2);

s32 ov59_0223B11C(Ov59JuiceFeedApp *arg0, s32 arg1, s32 arg2) {
    u16 temp_r1;
    temp_r1 = arg0->subState;
    switch (temp_r1) {
    case 0:
        ov59_0223BBB0(arg0);
        ov59_0223BBD4(arg0, 1);
        arg0->subState = (u16)((arg0->subState) + 1);
        break;

    case 1:
        if (TextPrinterCheckActive(arg0->printerId) == 0) {
            arg0->subState = 0U;
            return 1;
        }
        break;
    }
    return 0;
}
