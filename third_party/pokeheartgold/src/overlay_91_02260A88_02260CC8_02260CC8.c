#include "global.h"

#include "system.h"

s32 ov91_02260CE0(s32 *, s32, s32);

void ov91_02260CC8(s32 *arg0);

void ov91_02260CC8(s32 *arg0) {
    if (*arg0 != 0) {
        ov91_02260CE0(arg0, gSystem.touchX, gSystem.touchY);
    }
}
