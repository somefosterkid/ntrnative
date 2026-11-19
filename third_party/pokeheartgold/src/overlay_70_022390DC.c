#include "global.h"

#include "overlay_70_gtsapplication_state.h"

#include "unk_0203A3B0.h"
#include "sprite.h"

int ov70_02238E44(void);

extern void (*ov70_02246464[14])(GTSApplicationState *);

void ov70_022390DC(GTSApplicationState *param_1);

s32 ov70_02238E58(void *);
s32 ov70_022392E0(void *);
s32 ov70_022394A8(void *);
s32 ov70_022395C4(GTSApplicationState *);
s32 ov70_0223963C(void *);
s32 ov70_022390FC(GTSApplicationState *arg0);

void ov70_022390DC(GTSApplicationState * param_1) {
    s32 a0;

    a0 = ov70_02238E44();
    sub_0203A930(a0);
    ov70_02246464[param_1->currentScreenInstruction](param_1);
    return;
}

s32 ov70_022390FC(GTSApplicationState *arg0) {
    sub_0203A914();
    ov70_022394A8(arg0);
    ov70_0223963C(arg0);
    ov70_022395C4(arg0);
    ov70_022392E0(arg0->bgConfig);
    Sprite_SetDrawFlag(arg0->unk_F10, 0);
    ov70_02238E58(arg0);
    return 1;
}
