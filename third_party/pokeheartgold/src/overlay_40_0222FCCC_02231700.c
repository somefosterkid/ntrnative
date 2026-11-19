#include "global.h"

#include "error_handling.h"
#include "overlay_40.h"
#include "touchscreen.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

extern TouchscreenHitbox ov40_02245100;

extern TouchscreenHitbox ov40_02245104;

s32 ov40_02231700(void);

s32 ov40_02231700(void) {
    if (TouchscreenHitbox_TouchHeldIsIn(&ov40_02245100) != 0) {
        return 1;
    }
    if (TouchscreenHitbox_TouchHeldIsIn(&ov40_02245104) != 0) {
        return 2;
    }
    return 0;
}

s32 ov40_0222BF80(void *, s32);

s32 ov40_0223169C(void *);

void ov40_0223172C(UnkStruct_0208C06C *arg0) {
    GF_AssertFail();
    arg0->unk_14.unk_30 = 1;
    ov40_0222BF80(arg0, 0);
    ov40_0223169C(arg0);
}
