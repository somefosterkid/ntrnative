#include "global.h"

#include "error_handling.h"
#include "overlay_70_gtsapplication_state.h"

void *ov70_02241868(void *arg0, s32 arg1);

void *ov70_02241868(void *arg0, s32 arg1) {
    if (arg1 == 9) {
        return arg0 + 0x260 + (0x124 * *((s32 *)&((GTSApplicationState *)arg0)->selectedSearchResult));
    } else if (arg1 == 10) {
        return arg0 + 0x13C;
    } else if (arg1 == 8) {
        return arg0 + 0x13C;
    } else {
        GF_AssertFail();
        return NULL;
    }
}
