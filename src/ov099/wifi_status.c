// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov173/wifi_status.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/wifi_status.h"
#include "gfl/net.h"
#include "gfl/std.h"
#include "nitro/os.h"

// The status a player of the Wi-Fi Club shows the friends. The ROM has no name for this file: it is named after
// Black and White's wifi_status.c, and the functions' names are ours

// pokebw2's WIFI_STATUS_GetWifiStatus
u32 func_ov099_021c27d0(const WIFI_STATUS *status) {
    return status->wifiStatus;
}

// pokebw2's WIFI_STATUS_SetWifiStatus
void func_ov099_021c27d4(WIFI_STATUS *status, u8 wifiStatus) {
    status->wifiStatus = wifiStatus;
}

// pokebw2's WIFI_STATUS_GetGameMode
u32 func_ov099_021c27d8(const WIFI_STATUS *status) {
    return status->gameMode;
}

// pokebw2's WIFI_STATUS_SetGameMode
void func_ov099_021c27dc(WIFI_STATUS *status, u8 gameMode) {
    status->gameMode = gameMode;
}

// pokebw2's WIFI_STATUS_GetVChatStatus
u8 func_ov099_021c27e0(const WIFI_STATUS *status) {
    return status->vchatStatus;
}

// pokebw2's WIFI_STATUS_SetVChatStatus
void func_ov099_021c27e8(WIFI_STATUS *status, u8 vchatStatus) {
    status->vchatStatus = vchatStatus;
}

// pokebw2's WIFI_STATUS_IsVChatMac
BOOL func_ov099_021c27f8(const WIFI_STATUS *my, const WIFI_STATUS *friendStatus) {
    if (GFL_STD_MemCmp(my->myMac, friendStatus->vchatMac, WIFI_STATUS_MAC_SIZE) == 0) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WIFI_STATUS_SetVChatMac
void func_ov099_021c2810(WIFI_STATUS *my, const WIFI_STATUS *friendStatus) {
    my->callCounter++;
    MI_CpuCopy8(friendStatus->myMac, my->vchatMac, WIFI_STATUS_MAC_SIZE);
}

// pokebw2's WIFI_STATUS_ResetVChatMac
void func_ov099_021c282c(WIFI_STATUS *my) {
    u8 mac[WIFI_STATUS_MAC_SIZE] = { 0, 0, 0, 0, 0, 0 };

    MI_CpuCopy8(mac, my->vchatMac, WIFI_STATUS_MAC_SIZE);
}

// pokebw2's WIFI_STATUS_SetMyMac
void func_ov099_021c2850(WIFI_STATUS *my) {
    OS_GetMacAddress(my->myMac);
}

// pokebw2's WIFI_STATUS_GetUseVChat
BOOL func_ov099_021c285c(const WIFI_STATUS *status) {
    return status->useVChat;
}

// pokebw2's WIFI_STATUS_SetUseVChat
void func_ov099_021c2864(WIFI_STATUS *status, u8 useVChat) {
    status->useVChat = useVChat;
}

// pokebw2's WIFI_STATUS_GetCallCounter
u32 func_ov099_021c2874(const WIFI_STATUS *status) {
    return status->callCounter;
}
