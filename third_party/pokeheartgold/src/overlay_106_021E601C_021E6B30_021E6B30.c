#include "global.h"

#include "error_handling.h"
#include "overlay_106.h"
#include "overlay_106_legendary_cinematic.h"

s32 ov106_021E6B30(LegendaryCinematic *arg0, s32 *arg1);

s32 ov106_021E6B30(LegendaryCinematic *arg0, s32 *arg1) {
    if (*arg1 != 5) {
        GF_AssertFail();
    }
    arg0->state = 2;
    arg0->nextState = 4;
    return 2;
}
