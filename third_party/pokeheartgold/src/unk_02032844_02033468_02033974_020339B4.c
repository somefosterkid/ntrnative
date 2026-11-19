#include "global.h"

#include "wireless_manager.h"

void sub_020339B4(void *buffer, s32 size, s32 ggid, s32 tgid);

void sub_020339B4(void *buffer, s32 size, s32 ggid, s32 tgid) {
    if (_021D4124.sWirelessManager->state == 4) {
        WM_SetGameInfo((WMCallbackFunc)0x0, buffer, (u16)size, ggid, (u16)tgid, 1);
    }
    return;
}
