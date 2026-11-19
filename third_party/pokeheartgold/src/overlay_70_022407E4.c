#include "global.h"
#include "math_util.h"
#include "save.h"
#include "overlay_70_gtsapplication_state.h"
s32 ov70_02238D84(GTSApplicationState *, s32, s32);

s32 ov70_02238E50(GTSApplicationState *, s32, s32);

s32 ov70_02238F80(GTSApplicationState *);

s32 ov70_0224078C();

s32 ov70_022407E4(GTSApplicationState *arg0);

s32 ov70_02240D44(void *, s16, s16);
s32 ov70_02240838(void *arg0);
s32 ov70_02240854(GTSApplicationState *arg0);
s32 ov70_02240884(GTSApplicationState *arg0);

s32 ov70_022407E4(GTSApplicationState *arg0) {
    ov70_0224078C();
    ov70_02238D84(arg0, 0x25, 0x24);
    ov70_02238E50(arg0, 0, 0);
    ov70_02238F80(arg0);
    return 3;
}

s32 ov70_02241234(void *);

s32 ov70_0224080C(void *arg0);

s32 ov70_0224080C(void *arg0) {
    ov70_0224078C();
    ov70_02238D84(arg0, 0x25, 0x24);
    ov70_02238E50(arg0, 1, 0);
    ov70_02238F80(arg0);
    ov70_02241234(arg0);
    return 3;
}

s32 ov70_02240838(void *arg0) {
    ov70_02238D84(arg0, 0x25, 0x1E);
    ov70_02240D44(arg0, 0x21, 0x24);
    return 3;
}

s32 ov70_02240854(GTSApplicationState *arg0) {
    SetAllPCBoxesModified();
    Save_PrepareForAsyncWrite(*(SaveData **)((u8 *)(arg0->playerData) + 0x20), 2);
    arg0->currentScreenInstruction = 0x1F;
    arg0->unk_11C0 = (s32)((u16)((s32)LCRandom() % 60) + 2);
    return 3;
}

s32 ov70_02240884(GTSApplicationState *arg0) {
    arg0->unk_11C0 = (s32)(arg0->unk_11C0 - 1);
    if (arg0->unk_11C0 == 0) {
        arg0->currentScreenInstruction = 0x20;
    }
    return 3;
}
