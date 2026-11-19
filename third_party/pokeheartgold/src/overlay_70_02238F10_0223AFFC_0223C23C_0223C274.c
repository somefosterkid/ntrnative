#include "global.h"
struct GTSApplicationState;

#include "unk_02005D10.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02238D84(void *, s32, s32);

s32 ov70_0223CAC4(void *, s32, s32, s32, s32);

s32 ov70_0223C274(struct GTSApplicationState *arg0);

s32 ov70_0223C274(GTSApplicationState *arg0) {
    ov70_0223CAC4(arg0, 0x99, 4, 0, 0xF0F);
    ov70_02238D84(arg0, 0x16, 0x1C);
    *((s32 *)&arg0->unk_11C0) = 0;
    PlaySE(0x5F3U);
    return 3;
}
