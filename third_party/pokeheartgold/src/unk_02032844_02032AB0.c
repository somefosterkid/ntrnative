#include "global.h"

#include "wireless_manager.h"

void sub_02032AB0(void *arg0);

void sub_02032AB0(void *arg0) {
    s32 v1;
    u16 v2;
    u32 v0;

    v2 = ((WMCallback *)arg0)->errcode;
    if (v2 != 0) {
        sub_02032858(v2);
        sub_02032844(9);
        return;
    }
    v0 = *(u16 *)((u8 *)arg0 + 4) - 0xA;
    switch (v0) {
    case 1:
    case 2:
    case 3:
        break;
    case 0:
        if ((_021D4124.sWirelessManager->connectionType != 2) || (v1 = _021D4124.sWirelessManager->state, (v1 == 4)) || (v1 != 6)) {
            sub_02032844(4);
        }
        break;
    }
}
