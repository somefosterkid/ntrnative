#include "global.h"

#include "system.h"
#include "unk_02005D10.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02238D84(GTSApplicationState *, s32, s32);

s32 ov70_0223BD80(void);

s32 ov70_0223BD90(void *, s32);

s32 ov70_0223C2EC(GTSApplicationState *);

s32 ov70_0223C304(GTSApplicationState *);

s32 ov70_0223C420(GTSApplicationState *, s32);

s32 ov70_0223CAC4(GTSApplicationState *, s32, s32, s32, s32);

s32 ov70_0223BE84(void *arg0);

s32 ov70_0223BE84(void *arg0) {
    s32 v0;

    v0 = ov70_0223BD80();
    if (v0 != -1) {
        ov70_0223C420(arg0, v0);
        ov70_0223BD90(arg0, v0);
    } else {
        ov70_0223C304(arg0);
        if (gSystem.newKeys & 1) {
            ov70_0223BD90(arg0, ov70_0223C2EC(arg0));
        } else if (2 & gSystem.newKeys) {
            ov70_0223CAC4(arg0, 0xF, 1, 0, 0xF0F);
            ov70_02238D84(arg0, 0x15, 0x17);
            PlaySE(0x5DCU);
        }
    }
    return 3;
}
