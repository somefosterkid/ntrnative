// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/wdsscan.h, with Black's names.
#ifndef BWPORT_DWC_WDSSCAN_H
#define BWPORT_DWC_WDSSCAN_H

#include "types.h"
#include "dwc/connectinet.h"

// The WDS scan (TWL-DWC 5.3's dwc_wdsscan.c, a guessed name, in ARM9 main): when the auto connect fails, DWC listens
// for the beacons of a Nintendo Zone for three seconds and hands the connection the access point whose signal is the
// strongest. Only the connection of dwc/connectinet.h uses it. The ROM names none of it. The scan itself, on the
// wireless manager, is dwc/wds.h.

// Starts the scan, allocating its work: the result goes in settings, and dmaNo is the DMA channel of the wireless
// manager. The return value is always TRUE, and the caller ignores it
BOOL func_0205d91c(DWCInetApSettings *settings, u16 dmaNo);
// Takes a step of the scan: DWC_INET_SCAN_SCANNING while it runs, DWC_INET_SCAN_FOUND once it found an access point
// (in the settings), DWC_INET_SCAN_FAILED if it failed or was never started
int func_0205d98c(void);
// Asks the scan to stop: a scan that is waiting for its time to run out ends at the next step
void func_0205dc58(void);
// Frees the scan's work
void func_0205dc74(void);

#endif // BWPORT_DWC_WDSSCAN_H
