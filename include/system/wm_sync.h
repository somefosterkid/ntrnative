#ifndef BWPORT_SYSTEM_WM_SYNC_H
#define BWPORT_SYSTEM_WM_SYNC_H

#include "nitro/wm.h"

typedef struct WMStatus WMStatus;

// NitroSDK's WM_ReadStatus (lib/nitro/src/wm/wm_sync.c): copies the WM library's status into statusBuf
WMErrCode WM_ReadStatus(WMStatus *statusBuf);

#endif
