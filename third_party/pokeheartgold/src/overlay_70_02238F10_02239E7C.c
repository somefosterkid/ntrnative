#include "global.h"

#include "sprite.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02238E58(void *);

s32 ov70_02239F78(void *);

s32 ov70_0223A0C4(GTSApplicationState *arg0);

s32 ov70_0223A1A0(GTSApplicationState *);

s32 ov70_0223A224(void *);

s32 ov70_02239E7C(GTSApplicationState *arg0);

s32 ov70_02239E7C(GTSApplicationState *arg0) {
    ov70_0223A0C4(arg0);
    ov70_0223A224(arg0);
    ov70_0223A1A0(arg0);
    ov70_02239F78(arg0->bgConfig);
    Sprite_SetDrawFlag(arg0->unk_F10, 0);
    ov70_02238E58(arg0);
    return 1;
}
