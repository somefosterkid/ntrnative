#include "global.h"
#include "unk_02032844.h"
#include "wireless_manager.h"

void sub_02033214(void *arg0);

void sub_02033234(u32 ggid);
void sub_02033240(u16 *userGameInfo, u16 size);
u16 sub_02033264(void);
int sub_02033298(void);

void sub_02033214(void *arg0) {
    if (((WMCallback *)arg0)->errcode != 0) {
        sub_02032844(0xA);
        return;
    }
    WVR_TerminateAsync(NULL, NULL);
    sub_02032844(0);
}

void sub_02033234(u32 ggid) {
    _021D4124.sWirelessManager->parentParam.ggid = ggid;
}

void sub_02033240(u16 *userGameInfo, u16 size) {
    _021D4124.sWirelessManager->parentParam.userGameInfo = userGameInfo;
    _021D4124.sWirelessManager->parentParam.userGameInfoLength = size;
}

u16 sub_02033250(void) {
    return _021D4124.sWirelessManager->connectedBitmap;
}

u16 sub_02033264(void) {
    int cnt = 0, i;
    u16 connected = _021D4124.sWirelessManager->connectedBitmap;

    for (i = 0; i < 16; i++) {
        if (connected & 0x1) {
            cnt++;
        }

        connected = connected >> 1;
    }

    return cnt;
}

int sub_02033298(void) {
    return _021D4124.sWirelessManager->state;
}
