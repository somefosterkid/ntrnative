#include "global.h"

#include "unk_0203A3B0.h"
#include "sprite.h"
#include "overlay_70_gtsapplication_state.h"

int ov70_02238E44(void);

extern void (*ov70_022465A8[17])(GTSApplicationState *);

void ov70_0223CDD8(GTSApplicationState *param_1);

s32 ov70_02238E58(void *);
s32 ov70_0223CF48(void *);
s32 ov70_0223D208(void *);
s32 ov70_0223D378(GTSApplicationState *);
s32 ov70_0223D414(void *);
s32 ov70_02241380(void *);
s32 ov70_0223CDF8(GTSApplicationState *arg0);

void ov70_0223CDD8(GTSApplicationState * param_1) {
    s32 a0;

    a0 = ov70_02238E44();
    sub_0203A930(a0);
    ov70_022465A8[param_1->currentScreenInstruction](param_1);
    return;
}

s32 ov70_0223CDF8(GTSApplicationState *arg0) {
    sub_0203A914();
    if (GXS_GetMasterBrightness() != 0) {
        ov70_02241380(arg0);
    }
    ov70_0223D208(arg0);
    ov70_0223D414(arg0);
    ov70_0223D378(arg0);
    ov70_0223CF48(arg0->bgConfig);
    Sprite_SetDrawFlag(arg0->unk_F10, 0);
    ov70_02238E58(arg0);
    return 1;
}
