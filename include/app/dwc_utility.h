// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/dwc_utility.h, the lines in use so far.
#ifndef BWPORT_APP_DWC_UTILITY_H
#define BWPORT_APP_DWC_UTILITY_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"

// The Nintendo Wi-Fi Connection settings, overlay 182 in Black 2
#define OVERLAY_DWC_UTILITY OVERLAY_ID(16)

// pokebw2's DWC_UTILITY_PROC_FUNCTIONS
extern const GameProcFunctions data_ov016_021d77b8;

#endif // BWPORT_APP_DWC_UTILITY_H
