#include "global.h"
struct GTSApplicationState;

#include "unk_02005D10.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02238D84(void *, s32, s32);

s32 ov70_0223CAC4(void *, s32, s32, s32, s32);

s32 ov70_0223C19C(struct GTSApplicationState *arg0);

s32 ov70_0223C19C(GTSApplicationState *arg0) {
    if (*((s32 *)&arg0->searchResultCount) == 0) {
        ov70_0223CAC4(arg0, 0xE, 1, 0, 0xF0F);
        ov70_02238D84(arg0, 0x15, 1);
        PlaySE(0x5F3U);
    } else {
        ov70_0223CAC4(arg0, 0x1E, 1, 0, 0xF0F);
        ov70_02238D84(arg0, 0x15, 0x13);
        *((s32 *)&arg0->unk_11C0) = 0;
    }
    return 3;
}

s32 ov70_0223C1F8(struct GTSApplicationState *arg0);

s32 ov70_0223C1F8(GTSApplicationState *arg0) {
    *((s32 *)&arg0->unk_11C0) = (s32)(*((s32 *)&arg0->unk_11C0) + 1);
    if ((s32) *((s32 *)&arg0->unk_11C0) > 0x2D) {
        ov70_0223CAC4(arg0, 0x20, 1, 0, 0xF0F);
        ov70_02238D84(arg0, 0x15, 1);
        *((s16 *)&arg0->searchResultsVisible) = 1;
    }
    return 3;
}
