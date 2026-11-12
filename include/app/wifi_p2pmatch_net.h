// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wifi_p2pmatch_net.h: the lines in use so
// far.
#ifndef BWPORT_APP_WIFI_P2PMATCH_NET_H
#define BWPORT_APP_WIFI_P2PMATCH_NET_H

#include "types.h"

// The commands' high byte
#define WIFIP2PMATCH_NET_CMD_BASE 0xa00

// pokebw2's WifiP2PMatchNet_CommandInit
// work is the match board's work, which the handlers get
void func_ov099_021ba7d8(void *work);
// pokebw2's WifiP2PMatchNet_CommandExit
void func_ov099_021ba7f8(void *work);

#endif // BWPORT_APP_WIFI_P2PMATCH_NET_H
