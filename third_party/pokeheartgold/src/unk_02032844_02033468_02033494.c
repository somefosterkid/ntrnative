#include "global.h"

#include "wireless_manager.h"

s16 sub_02033494(u16 bitmap);

s16 sub_02033494(u16 bitmap) {
    s16 i;
    s16 channel = 0;
    u16 cnt = 0;
    u16 randChannel;

    for (i = 0; i < 16; i++) {
        if (bitmap & (1 << i)) {
            channel = (s16)(i + 1);
            cnt++;
        }
    }

    if (cnt <= 1) {
        return channel;
    }

    randChannel = (u16)((((_021D4124.sWirelessManager->rand = _021D4124.sWirelessManager->rand * 69069UL + 12345) & 0xFF) * cnt) / 0x100);
    channel = 1;

    for (i = 0; i < 16; i++) {
        if (bitmap & 1) {
            if (randChannel == 0) {
                return (s16)(i + 1);
            }

            randChannel--;
        }

        bitmap >>= 1;
    }

    return 0;
}
