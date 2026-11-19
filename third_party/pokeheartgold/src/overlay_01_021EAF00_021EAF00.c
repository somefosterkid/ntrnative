#include "global.h"

#include "error_handling.h"
#include "overlay_01_021EAF00.h"
#include "field_system.h"

s32 ov01_021EAF00(void *arg0) {
    s32 v0;

    v0 = *((s32 *)&((FieldSystem *)arg0)->bottomScreenType);
    if (v0 == 0) {
        GF_AssertFail();
    }
    if (v0 >= 7) {
        GF_AssertFail();
    }
    return v0 - 1;
}
