#include "global.h"

#include "unk_020210A0.h"
#include "overlay_59_juice_feed_app.h"

void ov59_0223AD20(void *arg0);


s32 ov59_0223AABC(Ov59JuiceFeedApp *arg0);

s32 ov59_0223AABC(Ov59JuiceFeedApp *arg0) {
    u16 temp_r1;

    temp_r1 = arg0->state;
    switch (temp_r1) {
    case 0:
        ov59_0223AD20(arg0);
        arg0->state = (u16)(arg0->state + 1);
        break;
    case 1:
        ov59_0223AD84(arg0);
        EnableTouchPad();
        InitializeTouchPad(1);
        arg0->state = 0U;
        return 1;
    }
    return 0;
}
