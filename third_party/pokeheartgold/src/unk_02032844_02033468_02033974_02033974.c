#include "global.h"

#include "wireless_manager.h"

s32 sub_02033974(void);

s32 sub_02033974(void) {
    if (_021D4124.sWirelessManager->state == 0xA) {
        return 1;
    }
    return 0;
}

s32 sub_02033990(void);

s32 sub_02033990(void) {
    if (_021D4124.sWirelessManager != NULL) {
        if (_021D4124.sWirelessManager->state == 2) {
            return 1;
        }
        return 0;
    }
    return 0;
}
