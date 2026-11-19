#include "global.h"
struct GTSApplicationState;

#include "unk_0203A3B0.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02238E44(void);

extern void (*ov70_02246658[40])(GTSApplicationState *);

void ov70_0223F9B4(GTSApplicationState *param_1);

s32 ov70_02238E58(void *);
s32 ov70_0223FB34(void *);
s32 ov70_0223FC30(GTSApplicationState *);
s32 ov70_0223FC58(void *);
s32 ov70_02241380(void *);
s32 ov70_0223F9D4(struct GTSApplicationState *arg0);

void ov70_0223F9B4(GTSApplicationState * param_1) {
    s32 a0;

    a0 = ov70_02238E44();
    sub_0203A930(a0);
    ov70_02246658[param_1->currentScreenInstruction](param_1);
    return;
}

s32 ov70_0223F9D4(GTSApplicationState *arg0) {
    if (GXS_GetMasterBrightness() != 0) {
        ov70_02241380(arg0);
    }
    ov70_0223FC58(arg0);
    ov70_0223FC30(arg0);
    ov70_0223FB34(*((void **)&arg0->bgConfig));
    ov70_02238E58(arg0);
    return 1;
}
