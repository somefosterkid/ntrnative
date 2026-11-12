// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/wih_dwc.h: the lines in use so far.
#ifndef BWPORT_SYSTEM_WIH_DWC_H
#define BWPORT_SYSTEM_WIH_DWC_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Stop and start the beacon timers' countdown, as the save does while it writes
void func_0201042c(void);
void func_02010444(void);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Allocates the copy of the console's Wi-Fi connection settings, and reads them
void setupDownloadPlayConfig(HeapID heapId);
void downloadPlayConfigForWifi(void);
BOOL func_0205d01c(void *work, const void *bssDesc, DWCApInfo *apInfo);

#endif // BWPORT_SYSTEM_WIH_DWC_H
