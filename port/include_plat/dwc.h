// port/include_plat/dwc.h: NitroDWC's umbrella header as pret pokeplatinum includes it (<dwc.h>), on the port's own
// lib/dwc headers (Black's DWC: the online side is stubbed on the PC, so only the declarations are needed), plus the
// type names those spell as plain ints/enums and Platinum's code spells as SDK types, and the DWC 2.2 names of
// dwc_gen4.h
#ifndef BWPORT_PLAT_DWC_H
#define BWPORT_PLAT_DWC_H

// libvct under Platinum's names first: its guard is Black's dwc/vct.h's, which it replaces
#include <vct.h>

// four of DWC 2.2's friends-match calls take other arguments than Black's DWC's: Black's stay under other names while
// its headers are read, and dwc_gen4.h declares Platinum's
#define DWC_InitFriendsMatch DWCi_BlackInitFriendsMatch
#define DWC_ConnectToAnybodyAsync DWCi_BlackConnectToAnybodyAsync
#define DWC_SetupGameServer DWCi_BlackSetupGameServer
#define DWC_ConnectToGameServerAsync DWCi_BlackConnectToGameServerAsync

#include <dwc/dwc.h>
#include <dwc/base.h>
#include <dwc/error.h>
#include <dwc/memfunc.h>
#include <dwc/connectinet.h>
#include <dwc/apinfo.h>
#include <dwc/ac.h>
#include <dwc/nd.h>
#include <dwc/utility.h>

#undef DWC_InitFriendsMatch
#undef DWC_ConnectToAnybodyAsync
#undef DWC_SetupGameServer
#undef DWC_ConnectToGameServerAsync

typedef int DWCErrorType;   // DWC_ETYPE_*
typedef int DWCAllocType;   // DWC_ALLOCTYPE_* (dwc/memfunc.h)
#define DWC_ERROR_NUM (DWC_ERROR_ND_ANY + 1)

#include "dwc_gen4.h"

#endif // BWPORT_PLAT_DWC_H
