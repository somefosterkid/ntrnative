#ifndef BWPORT_SYSTEM_DWC_BACKUP_H
#define BWPORT_SYSTEM_DWC_BACKUP_H

#include "types.h"
#include "dwc/bm.h"

// Black's main 0x020584e0-0x020589d0 (lib/dwc/src/bm/dwc_backup.c)
void DWCi_BACKUPlConvMaskAddr(u32 param_1, u8 *param_2);
BOOL func_02058784(DWCMemPage *pages, const BOOL flags[4], void *scratch);
BOOL func_02058770(DWCMemPage *pages);
// The Wi-Fi info as the backup's 14 bytes (a static buffer)
struct DWCWiFiInfo;
const u8 *DWCi_BACKUPlConvWifiInfo(const struct DWCWiFiInfo *info);

#endif // BWPORT_SYSTEM_DWC_BACKUP_H
