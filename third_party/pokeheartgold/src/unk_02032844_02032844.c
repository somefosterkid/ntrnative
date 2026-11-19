#include "global.h"

#include "wireless_manager.h"

void sub_02032844(int state) {
    _021D4124.sWirelessManager->state = state;
    return;
}

void sub_02032858(int errorCode) {
    if (1 < _021D4124.sWirelessManager->state - 9U) {
        _021D4124.sWirelessManager->errorCode = errorCode;
    }
    return;
}
