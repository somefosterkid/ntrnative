#include "global.h"
struct GTSApplicationState;

#include "unk_02037C94.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02238E50(void *, s32, s32);

s32 ov70_02240640(GTSApplicationState *arg0);

void ov70_0224078C(struct GTSApplicationState *arg0);

s32 ov70_02240640(GTSApplicationState *arg0) {
    arg0->isPokemonListed = 1;
    ov70_02238E50(arg0, 9, 7);
    arg0->currentScreenInstruction = 0x24;
    return 3;
}

s32 ov70_02240658(GTSApplicationState *arg0);

s32 ov70_02240658(GTSApplicationState *arg0) {
    arg0->isPokemonListed = 0;
    ov70_02238E50(arg0, 9, 8);
    arg0->currentScreenInstruction = 0x24;
    return 3;
}

s32 ov70_02240670(GTSApplicationState *arg0);

s32 ov70_02240670(GTSApplicationState *arg0) {
    ov70_02238E50(arg0, 9, 9);
    arg0->currentScreenInstruction = 0x24;
    return 3;
}

s32 ov70_02240684(GTSApplicationState *arg0);

s32 ov70_02240684(GTSApplicationState *arg0) {
    arg0->isPokemonListed = 0;
    ov70_02238E50(arg0, 9, 0xA);
    arg0->currentScreenInstruction = 0x1E;
    return 3;
}

s32 ov70_022380EC(void);

s32 ov70_0224069C(GTSApplicationState *arg0);

s32 ov70_0224069C(GTSApplicationState *arg0) {
    ov70_022380EC();
    arg0->currentScreenInstruction = 0x17;
    arg0->networkTimeoutCounter = 0;
    return 3;
}

s32 ov70_02237F38(void);

s32 ov70_02237F58(void);

s32 ov70_022404D4(void *);

s32 ov70_022406B8(GTSApplicationState *arg0);

s32 ov70_022406B8(GTSApplicationState *arg0) {
    u32 v0;

    if (ov70_02237F38() != 0) {
        v0 = ov70_02237F58() + 0xF;
        arg0->networkTimeoutCounter = 0;
        switch (v0) {
        case 15:
            ov70_022404D4(arg0);
            break;
        case 12:
            ov70_022404D4(arg0);
        case 11:
            ov70_022404D4(arg0);
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

s32 ov70_02238D84(void *, s32, s32);

s32 ov70_02238F80(void *);

s32 ov70_02241234(void *);

s32 ov70_02244FA4(void *, MsgData *, s32, s32, s32);

s32 ov70_02240748(struct GTSApplicationState *arg0);

s32 ov70_02240748(GTSApplicationState *arg0) {
    ov70_02244FA4(arg0, arg0->gtsMessageLoader, 0x93, 1, 0xF0F);
    ov70_02238D84(arg0, 0x25, 0x24);
    ov70_02238E50(arg0, 1, 0);
    ov70_02238F80(arg0);
    ov70_02241234(arg0);
    return 3;
}

void ov70_0224078C(GTSApplicationState *arg0) {
    s32 v1;
    u32 v0;

    v1 = 0x9A;
    v0 = *((s32 *)&arg0->commsErrorMessage) + 0xF;
    switch (v0) {
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        v1 = 0x1A;
        break;
    case 14:
        v1 = 0x97;
        break;
    case 1:
    case 13:
        v1 = 0x9C;
        break;
    }
    ov70_02244FA4(arg0, arg0->gtsMessageLoader, v1, 1, 0xF0F);
}
