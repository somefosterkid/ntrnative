#include "global.h"
struct GTSApplicationState;

#include "pm_string.h"
#include "error_handling.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_0223FC58(struct GTSApplicationState *arg0);

s32 ov70_02238D84(void *, s32, s32);
s32 ov70_02238F64(GTSApplicationState *);
s32 ov70_02244FA4(void *, void *, s32, s32, s32);
s32 ov70_0223FC68(struct GTSApplicationState *arg0);

void ov70_0223FC58(GTSApplicationState *arg0) {
    String_Delete(arg0->genericMessageBuffer);
}

s32 ov70_0223FC68(GTSApplicationState *arg0) {
    u32 v0;

    v0 = *((u32 *)&arg0->screenArgument);
    switch (v0) {
    case 7:
        ov70_02244FA4(arg0, arg0->gtsMessageLoader, 0x18, 1, 0xF0F);
        ov70_02238D84(arg0, 0x25, 2);
        break;
    case 8:
        ov70_02244FA4(arg0, arg0->gtsMessageLoader, 0x18, 1, 0xF0F);
        ov70_02238D84(arg0, 0x25, 7);
        break;
    case 9:
        ov70_02244FA4(arg0, arg0->gtsMessageLoader, 0x18, 1, 0xF0F);
        ov70_02238D84(arg0, 0x25, 0xC);
        break;
    case 10:
        ov70_02244FA4(arg0, arg0->gtsMessageLoader, 0x18, 1, 0xF0F);
        ov70_02238D84(arg0, 0x25, 0x12);
        *((s32 *)&arg0->fadeBothScreens) = 1;
        break;
    case 11:
        ov70_02244FA4(arg0, arg0->gtsMessageLoader, 0x18, 0, 0xF0F);
        *((s32 *)&arg0->currentScreenInstruction) = 0x18;
        break;
    case 12:
        ov70_02244FA4(arg0, arg0->gtsMessageLoader, 0x94, 1, 0xF0F);
        *((s32 *)&arg0->nextScreen) = 1;
        *((s32 *)&arg0->currentScreenInstruction) = 0x1D;
        break;
    default:
        GF_AssertFail();
        break;
    }
    ov70_02238F64(arg0);
    return 3;
}
