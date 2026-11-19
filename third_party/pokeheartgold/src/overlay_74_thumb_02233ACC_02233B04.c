#include "global.h"

#include "pm_agb_save.h"

void *ov74_02233B04(s32 arg0);

void *ov74_02233B04(s32 arg0) {
    if (arg0 == 0) {
        return ov74_0223D33C.buffer + 0x1000;
    }
    if ((arg0 >= 1) && (arg0 <= 4)) {
        return ov74_0223D33C.buffer + 0x2000 + (0xF80 * (arg0 - 1));
    }
    if ((arg0 >= 5) && (arg0 < 0xE)) {
        return ov74_0223D33C.buffer + 0x6000 + (0xF80 * (arg0 - 5));
    }
    return 0;
}
