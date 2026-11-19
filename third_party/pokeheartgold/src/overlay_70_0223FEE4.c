#include "global.h"

#include "overlay_70_gtsapplication_state.h"
#include "unk_02037C94.h"

void ov70_02238008(void *);

void ov70_02240D44(GTSApplicationState *, s16, s16);

u32 ov70_0223FEE4(GTSApplicationState *param_1);

s32 ov70_02237F38(void);
s32 ov70_02237F58(void);
s32 ov70_02240A7C(void *, void *, s32, int);
u16 sub_0202DBA0(void *);
s32 ov70_0223FF10(GTSApplicationState *arg0);

u32 ov70_0223FEE4(GTSApplicationState *param_1) {
    ov70_02238008(&param_1->receivedListing);
    param_1->currentScreenInstruction = 8;
    param_1->networkTimeoutCounter = 0;
    ov70_02240D44(param_1, 9, 0xb);
    return 3;
}

s32 ov70_0223FF10(GTSApplicationState *arg0) {
    s32 v0;
    s8 v2;
    u32 v1;

    if (ov70_02237F38() != 0) {
        v0 = ov70_02237F58();
        arg0->networkTimeoutCounter = 0;
        v1 = v0 + 0xF;
        switch (v1) {
        case 15:
            v2 = arg0->receivedListing.exchangedFromRemote;
            if (v2 != 0) {
                arg0->currentScreenInstruction = 0x18;
            } else {
                ov70_02240A7C(arg0, &arg0->receivedListing, sub_0202DBA0(*(void **)arg0->playerData), v2);
                arg0->currentScreenInstruction = 0x1E;
            }
            break;
        case 12:
            arg0->currentScreenInstruction = 0x26;
            break;
        case 11:
            arg0->currentScreenInstruction = 0x26;
            break;
        case 3:
            arg0->commsErrorMessage = v0;
            arg0->currentScreenInstruction = 0x27;
            break;
        case 0:
        case 1:
        case 13:
            arg0->commsErrorMessage = v0;
            arg0->currentScreenInstruction = 0x26;
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
