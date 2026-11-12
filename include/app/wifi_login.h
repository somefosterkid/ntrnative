// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wifi_login.h, the lines in use so far.
#ifndef BWPORT_APP_WIFI_LOGIN_H
#define BWPORT_APP_WIFI_LOGIN_H

#include "types.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "struct_decls.h"

// The caller's callback, run with the message window while the login waits: returns 0 to finish, 1 to show an error
// and 2 to keep waiting; param is the param's unk24
typedef struct WifiLoginMessage WifiLoginMessage;
typedef u32 (*WifiLoginCallback)(WifiLoginMessage *msg, void *param);

// Logging in to and out of Nintendo Wi-Fi Connection, overlay 190 in Black 2
#define OVERLAY_WIFILOGIN OVERLAY_ID(115)

typedef struct {
    GameData *gameData;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    void *buffer;
    // Set to log in again after an error
    u32 unk14;
    u32 unk18;
    u32 result;
    WifiLoginCallback unk20;
    // What unk20 is passed (worldtrade_enter.c's WorldTradeWork)
    void *unk24;
} WifiLoginParam;

typedef struct {
    GameData *gameData;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
    // A callback as the login's unk20 and what it is passed (worldtrade_enter.c's), which Black's logout never calls
    WifiLoginCallback unk14;
    void *unk18;
} WifiLogoutParam;

// pokebw2's WIFILOGIN_PROC_FUNCTIONS and WIFILOGOUT_PROC_FUNCTIONS
extern const GameProcFunctions data_ov115_021d6338;
extern const GameProcFunctions data_ov115_021d654c;

#endif // BWPORT_APP_WIFI_LOGIN_H
