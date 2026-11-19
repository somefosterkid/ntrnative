#include "global.h"

#include "overlay_112_sprite_app.h"

void ov112_021F15E4(Ov112SpriteApp *arg0);

void ov112_021F15E4(Ov112SpriteApp *arg0) {
    u32 spC[4];
    u32 sp0[3];
    Ov112Record *p;
    s32 *q;

    p = arg0->records[arg0->curIdx];
    q = spC;
    q[0] = 0;
    q[1] = 0;
    q[2] = 0;
    q[3] = 0;
    q = sp0;
    q[0] = 0;
    q[1] = 0;
    q[2] = 0;
    RTC_ConvertSecondToDateTime(&arg0->date, &arg0->time, p->timestamp);
}
