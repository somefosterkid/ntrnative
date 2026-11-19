#include "global.h"

#include "wireless_manager.h"

void sub_020339F0(void *arg0);

void sub_020339F0(void *arg0) {
    if (((WMCallback *)arg0)->errcode == 0) {
        _021D4124.sWirelessManager->setEntry = 1;
    }
}
