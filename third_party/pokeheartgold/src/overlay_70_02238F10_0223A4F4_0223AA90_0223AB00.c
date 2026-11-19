#include "global.h"

#include "overlay_70_gtsapplication_state.h"

#include "bg_window.h"
#include "sprite.h"
struct GTSApplicationState;

void ov70_02238E58(void *arg0);

void ov70_0223ABD8(BgConfig *arg0);

void ov70_0223ACE4(void *arg0);

void ov70_0223AF30(void *arg0);

s32 ov70_0223AE40(struct GTSApplicationState *);

s32 ov70_0223AB00(GTSApplicationState *arg0);

s32 ov70_0223AB00(GTSApplicationState *arg0) {
    arg0->unk_1208 = 0;
    ov70_0223ACE4(arg0);
    ov70_0223AF30(arg0);
    ov70_0223AE40(arg0);
    ov70_0223ABD8(arg0->bgConfig);
    Sprite_SetDrawFlag(arg0->unk_F10, 0);
    ov70_02238E58(arg0);
    return 1;
}
