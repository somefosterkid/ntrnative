#include "global.h"

#include "system.h"
#include "error_handling.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02238E50(void *, s32, s32);

void ov70_0223ED8C(GTSApplicationState *arg0);

s32 ov70_022420C4(void *, s16);
s32 ov70_0223EDB0(GTSApplicationState *arg0);
s32 ov70_0223EDCC(void);
s32 ov70_0223EDD8(void);
s32 ov70_0223EDE4(void *arg0, s32 arg1);

void ov70_0223ED8C(GTSApplicationState *arg0) {
    if (2 & gSystem.newKeys) {
        ov70_02238E50(arg0, 5, 5);
        arg0->currentScreenInstruction = 2;
    }
    arg0->currentScreenInstruction = 3;
}

s32 ov70_0223EDB0(GTSApplicationState *arg0) {
    ov70_022420C4(arg0->unk_11A8, 0);
    arg0->currentScreenInstruction = 6;
    return 3;
}

s32 ov70_0223EDCC(void) {
    GF_AssertFail();
    return 3;
}

s32 ov70_0223EDD8(void) {
    GF_AssertFail();
    return 3;
}

s32 ov70_0223EDE4(void *arg0, s32 arg1) {
    switch (arg1) {
    case 0x0:
        *(s8 *)((u8 *)arg0 + 2) = 1;
        return 1;
    case 0xFE:
        *(s8 *)((u8 *)arg0 + 2) = 2;
        return 1;
    case 0xFF:
        *(s8 *)((u8 *)arg0 + 2) = 3;
        return 1;
    default:
        return 0;
    }
}
