#include "global.h"

#include "unk_020210A0.h"
#include "overlay_59_juice_feed_app.h"

s32 ov59_0223ACD0(void);

s32 ov59_0223AD58(Ov59JuiceFeedApp *);

s32 ov59_0223AB00(Ov59JuiceFeedApp *arg0);

s32 ov59_0223AB00(Ov59JuiceFeedApp *arg0) {
    switch (arg0->state) {
    case 0:
        DisableTouchPad();
        ov59_0223AD58(arg0);
        arg0->state = (u16)((arg0->state) + 1);
        break;

    case 1:

    default:
        ov59_0223ACD0();
        arg0->state = 0U;
        return 1;
    }
    return 0;
}
