#include "global.h"
struct GTSApplicationState;
#include "screen_fade.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_0223F960(u8 *arg0, s32 arg1, s16 arg2, s16 arg3);

s32 ov70_0223FA08(void *);
s32 ov70_0223FB60(void *);
s32 ov70_0223FBF4(void *);
s32 ov70_0223FC40(GTSApplicationState *arg0);
s32 ov70_02241358(void *);
void *ov70_02245124(void *);
s32 ov70_0223F96C(struct GTSApplicationState *arg0);

void ov70_0223F960(u8 *arg0, s32 arg1, s16 arg2, s16 arg3) {
    void *v0;

    v0 = (void *)(arg0 + (arg1 * 2));
    *(s16 *)((u8 *)v0 + 4) = arg2;
    *(s16 *)((u8 *)v0 + 0x18) = arg3;
}

s32 ov70_0223F96C(GTSApplicationState *arg0) {
    ov70_0223FC40(arg0);
    ov70_0223FA08(*((void **)&arg0->bgConfig));
    ov70_0223FB60(arg0);
    ov70_0223FBF4(arg0);
    ov70_02241358(arg0);
    BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_61);
    ov70_02245124(arg0);
    *((s32 *)&arg0->currentScreenInstruction) = 0;
    return 2;
}
