#include "global.h"

#include "wireless_manager.h"

BOOL sub_020335D4(int connectionType);

BOOL sub_02033528(void *buffer, int connectionType) {
    WirelessManager *manager;
#if defined(SDK_PORT) || defined(NONMATCHING)
    manager = __builtin_align_up(buffer, 32);
#else
    u32 addr = (u32)buffer;
    u32 misalign = (u32)(addr << 0x1B) >> 0x1B;
    if (misalign != 0) {
        addr += 0x20 - misalign;
    }
    manager = (WirelessManager *)addr;
#endif
    _021D4124.sWirelessManager = manager;
    manager->recvBufferSize = 0;
    _021D4124.sWirelessManager->sendBufferSize = 0;
    _021D4124.sWirelessManager->recvFunc = NULL;
    _021D4124.sWirelessManager->aid = 0;
    _021D4124.sWirelessManager->connectedBitmap = 1;
    _021D4124.sWirelessManager->errorCode = 0;
    _021D4124.sWirelessManager->state = 0;
    _021D4124.sWirelessManager->parentParam.userGameInfo = NULL;
    _021D4124.sWirelessManager->parentParam.userGameInfoLength = 0;
    _021D4124.sWirelessManager->unusedCallback_1390 = NULL;
    _021D4124.sWirelessManager->numConnectionsMax = 8;
    _021D4124.sWirelessManager->pauseConnectionClient = 0;
    _021D4124.sWirelessManager->pauseConnection = 0;
    if (sub_020335D4(connectionType) == 0) {
        return 0;
    }
    return 1;
}

u32 sub_020335B4(void);

u32 sub_020335B4(void) {
    return sizeof(WirelessManager) + 32;
}

void sub_020335BC(void *arg0);

void sub_020335BC(void *arg0) {
    if (((WMCallback *)arg0)->errcode == 8) {
        sub_02032844(WIRELESS_STATE_ERROR);
        sub_02032858(0x19);
    }
}
