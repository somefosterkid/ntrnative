#include "global.h"

#include "unk_02037C94.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_0223826C(void);

s32 ov70_02240180(GTSApplicationState *arg0);

void ov70_02238058(void *);
u32 ov70_02240220(GTSApplicationState *param_1);

s32 ov70_02240180(GTSApplicationState *arg0) {
    ov70_0223826C();
    arg0->currentScreenInstruction = 0xF;
    arg0->networkTimeoutCounter = 0;
    return 3;
}

s32 ov70_02237F38(void);

s32 ov70_02237F58(void);

s32 ov70_0224019C(GTSApplicationState *arg0);

s32 ov70_0224019C(GTSApplicationState *arg0) {
    u32 v0;

    if (ov70_02237F38() != 0) {
        v0 = ov70_02237F58() + 0xF;
        arg0->networkTimeoutCounter = 0;
        switch (v0) {
        case 15:
            arg0->currentScreenInstruction = 0x21;
            break;
        case 10:
            sub_02039AD8(3);
            break;
        case 0:
        case 1:
        case 3:
        case 13:
            sub_02039AD8(4);
            break;
        case 2:
            sub_020399EC();
            break;
        }
    } else {
        arg0->networkTimeoutCounter = (s32)(arg0->networkTimeoutCounter + 1);
        if (arg0->networkTimeoutCounter == 0xE10) {
            sub_020399EC();
        }
    }
    return 3;
}

u32 ov70_02240220(GTSApplicationState * param_1) {
    ov70_02238058(&param_1->receivedListing);
    param_1->currentScreenInstruction = 0x19;
    param_1->networkTimeoutCounter = 0;
    return 3;
}
