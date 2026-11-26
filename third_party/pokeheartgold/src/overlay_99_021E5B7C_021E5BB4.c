#include "global.h"

#include "error_handling.h"
#include "overlay_99.h"

s32 ov99_021E5BB4(PokeathlonCourseRecordArgs *arg0, s32 arg1) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    if (ov99_021E5B90(arg0, arg1) != 0) {
        return 1;
    }
    return 0;
}
