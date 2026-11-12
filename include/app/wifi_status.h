// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wifi_status.h: the lines in use so far.
#ifndef BWPORT_APP_WIFI_STATUS_H
#define BWPORT_APP_WIFI_STATUS_H

#include "types.h"

#define WIFI_STATUS_MAC_SIZE 6

// Black 2's; matching code confirms the offsets of profileId, trainerId, vchatMac, myMac, trainerView, gameMode,
// wifiStatus, vchatStatus, gender, useVChat and callCounter in Black
typedef struct {
    // The GameSpy profile ID
    s32 profileId;
    u32 trainerId;
    // The MAC address of the friend the player is in a voice chat with
    u8 vchatMac[WIFI_STATUS_MAC_SIZE];
    u8 myMac[WIFI_STATUS_MAC_SIZE];
    u8 trainerView;
    u8 version;
    u8 language;
    u8 gameMode;
    u8 wifiStatus;
    u8 country;
    u8 province;
    u8 unk1B_0 : 1;
    u8 vchatStatus : 1;
    u8 gender : 2;
    u8 useVChat : 1;
    u8 unk1B_5 : 3;
    // How many times the player has set a voice chat partner
    u8 callCounter;
    u8 unk1D[3];
} WIFI_STATUS;

// pokebw2's WIFI_STATUS_SetWifiStatus
void func_ov099_021c27d4(WIFI_STATUS *status, u8 wifiStatus);
// pokebw2's WIFI_STATUS_SetGameMode
void func_ov099_021c27dc(WIFI_STATUS *status, u8 gameMode);
// pokebw2's WIFI_STATUS_GetVChatStatus
u8 func_ov099_021c27e0(const WIFI_STATUS *status);
// pokebw2's WIFI_STATUS_SetVChatStatus
void func_ov099_021c27e8(WIFI_STATUS *status, u8 vchatStatus);
// pokebw2's WIFI_STATUS_IsVChatMac
// Whether the friend is in a voice chat with the player
BOOL func_ov099_021c27f8(const WIFI_STATUS *my, const WIFI_STATUS *friendStatus);
// pokebw2's WIFI_STATUS_SetVChatMac
void func_ov099_021c2810(WIFI_STATUS *my, const WIFI_STATUS *friendStatus);
// pokebw2's WIFI_STATUS_ResetVChatMac
void func_ov099_021c282c(WIFI_STATUS *my);
// pokebw2's WIFI_STATUS_SetMyMac
void func_ov099_021c2850(WIFI_STATUS *my);
// pokebw2's WIFI_STATUS_GetUseVChat
BOOL func_ov099_021c285c(const WIFI_STATUS *status);
// pokebw2's WIFI_STATUS_SetUseVChat
void func_ov099_021c2864(WIFI_STATUS *status, u8 useVChat);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's WIFI_STATUS_GetWifiStatus
u32 func_ov099_021c27d0(const WIFI_STATUS *status);
// pokebw2's WIFI_STATUS_GetGameMode
u32 func_ov099_021c27d8(const WIFI_STATUS *status);
// pokebw2's WIFI_STATUS_GetCallCounter
u32 func_ov099_021c2874(const WIFI_STATUS *status);

#endif // BWPORT_APP_WIFI_STATUS_H
