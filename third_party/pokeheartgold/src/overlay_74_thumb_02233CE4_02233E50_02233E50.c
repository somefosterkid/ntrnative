#include "global.h"

#include "pm_agb_save.h"

s32 ov74_02233E50(void *arg0);

s32 ov74_02233E50(void *arg0) {
    s32 v0;

    if (*(s32 *)((u8 *)arg0 + 8) == 0) {
        v0 = ov74_0223D33C.sectorsWritten + 1;
        ov74_0223D33C.sectorsWritten = v0;
        if (v0 >= 0xE) {
            ov74_0223D33C.sectorsWritten = 0;
            ov74_0223D33C.writeStatus = 0;
            return 0;
        }
        if (v0 == 0xD) {
            ov74_0223D33C.writeStatus = 2;
            return 0;
        }
        ov74_0223D33C.writeStatus = 3;
    } else {
        ov74_0223D33C.writeStatus = 4;
    }
    return 0;
}
