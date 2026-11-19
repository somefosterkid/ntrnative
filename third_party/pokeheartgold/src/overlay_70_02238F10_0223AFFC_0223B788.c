#include "global.h"

#include "sprite.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02238E58(void *);

s32 ov70_0223B8B4(void *);

s32 ov70_0223BAAC(void *);

s32 ov70_0223BC2C(GTSApplicationState *);

s32 ov70_0223BCD0(void *);

s32 ov70_0224212C(void *);

s32 ov70_0223B788(GTSApplicationState *arg0);

s32 ov70_0223B788(GTSApplicationState *arg0) {
    arg0->unk_1208 = 0;
    ov70_0224212C(arg0->unk_11A8);
    ov70_0223BAAC(arg0);
    ov70_0223BCD0(arg0);
    ov70_0223BC2C(arg0);
    ov70_0223B8B4(((void *)arg0->bgConfig));
    Sprite_SetDrawFlag(arg0->unk_F10, 0);
    ov70_02238E58(arg0);
    return 1;
}
