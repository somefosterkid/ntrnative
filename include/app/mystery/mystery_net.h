// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/mystery/mystery_net.h: the lines in use
// so far.
#ifndef BWPORT_APP_MYSTERY_MYSTERY_NET_H
#define BWPORT_APP_MYSTERY_MYSTERY_NET_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The states MysteryNet_ChangeState starts and MysteryNet_GetState gives
enum {
    MYSTERY_NET_STATE_IDLE,
    MYSTERY_NET_STATE_WIRELESS_START,
    MYSTERY_NET_STATE_WIRELESS_READY,
    MYSTERY_NET_STATE_WIRELESS_END,
    MYSTERY_NET_STATE_WIFI,
    // Asks the Wi-Fi download to stop
    MYSTERY_NET_STATE_WIFI_CANCEL,
    MYSTERY_NET_STATE_WIFI_END,
    MYSTERY_NET_STATE_BEACON_START,
    MYSTERY_NET_STATE_BEACON_WAIT,
    MYSTERY_NET_STATE_BEACON_END,
    MYSTERY_NET_STATE_IRC_START,
    MYSTERY_NET_STATE_IRC_WAIT,
    MYSTERY_NET_STATE_IRC_END,
};

// Black 2's; twinned code confirms the values of MYSTERY_NET_RECV_OK and MYSTERY_NET_RECV_ERROR in Black
// What MysteryNet_GetRecvData gives
enum {
    MYSTERY_NET_RECV_NONE,
    MYSTERY_NET_RECV_OK,
    MYSTERY_NET_RECV_ERROR,
};

// Black 2's; twinned code confirms the values of MYSTERY_NET_ERROR_NONE and MYSTERY_NET_ERROR_FATAL in Black
// What MysteryNet_GetError gives
enum {
    MYSTERY_NET_ERROR_NONE,
    MYSTERY_NET_ERROR_FATAL,
    MYSTERY_NET_ERROR_DISCONNECT,
};

// pokebw2's MysteryNet_Create
MysteryNet *func_ov121_021de2a0(SaveControl *save, HeapID heapId);
// pokebw2's MysteryNet_ChangeState
void func_ov121_021de308(MysteryNet *net, u32 state);
// pokebw2's MysteryNet_GetState
u32 func_ov121_021de3e0(MysteryNet *net);
// pokebw2's MysteryNet_GetBeaconFlags
u32 func_ov121_021de4d8(MysteryNet *net);
// pokebw2's MysteryNet_GetRecvData
u32 func_ov121_021de4dc(MysteryNet *net, void *buffer, u32 size);
// pokebw2's MysteryNet_GetError
u32 func_ov121_021de500(MysteryNet *net);
// pokebw2's MysteryNet_ClearError
void func_ov121_021de550(MysteryNet *net);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's MysteryNet_Delete
void func_ov121_021de2f0(MysteryNet *net);
// pokebw2's MysteryNet_Main
void func_ov121_021de300(MysteryNet *net);

#endif // BWPORT_APP_MYSTERY_MYSTERY_NET_H
