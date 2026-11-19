#include "global.h"

#include "heap.h"
#include "overlay_74_mystery_gift_statics.h"
#include "unk_02034B0C.h"
#include "unk_0203A3B0.h"

void ov74_0222CE6C(u32 arg0);

void ov74_0222CE6C(u32 arg0) {
    switch (arg0) {
    case 0:
        ov74_0223D0A8.state = WIRELESS_DISTRIBUTION_STATE_40;
        return;
    case 1:
        ov74_0223D0A8.state = WIRELESS_DISTRIBUTION_STATE_41;
        return;
    case 2:
        ov74_0223D0A8.state = WIRELESS_DISTRIBUTION_STATE_42;
        return;
    case 3:
        if (ov74_0223D0A8.state == WIRELESS_DISTRIBUTION_STATE_42) {
            ov74_0223D0A8.state = WIRELESS_DISTRIBUTION_STATE_44;
            return;
        }
        ov74_0223D0A8.state = WIRELESS_DISTRIBUTION_STATE_47;
        return;
    case 4:
        ov74_0223D0A8.state = WIRELESS_DISTRIBUTION_STATE_48;
    default:
        return;
    }
}

void ov74_0222CEC0(void);

void ov74_0222CEC0(void) {
    sub_02034DE0();
    Heap_Free(ov74_0223D0A8.workBuf);
    sub_0203A914();
    ov74_0223D0A8.workBuf = NULL;
}
