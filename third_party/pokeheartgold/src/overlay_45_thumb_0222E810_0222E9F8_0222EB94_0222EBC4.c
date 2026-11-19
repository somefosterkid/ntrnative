#include "global.h"

#include "error_handling.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

s32 ov45_0222EBC4(void);

s32 ov45_0222EBC4(void) {
    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    if (_022577C0->unk_12E == 0) {
        return 1;
    }
    return 0;
}
