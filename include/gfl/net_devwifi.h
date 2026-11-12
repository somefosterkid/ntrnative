// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/net_devwifi.h: its types and constants,
// and the prototypes of Black's named functions.
#ifndef BWPORT_GFL_NET_DEVWIFI_H
#define BWPORT_GFL_NET_DEVWIFI_H

#include "types.h"
#include "gfl/net.h"

// pokebw2's NetDevWifi_GetTable
const GFLNetDevTable *func_ov009_0215536c(void);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// An ARM function of main past the network library: gets the last error of DWC and its type, returning its result code
int DWC_GetLastErrorEx(int *code, int *type);

#endif // BWPORT_GFL_NET_DEVWIFI_H
