// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/src/base/dwc_wdsscan.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "dwc/memfunc.h"
#include "dwc/wds.h"
#include "dwc/wdsscan.h"
#include "nitro/mi.h"
#include "nitro/os.h"

// DWC's WDS scan (TWL-DWC 5.3's dwc_wdsscan.c, a guessed name; the ROM names none of it, and the file is ARM code in
// ARM9 main like the rest of the library's base): after the auto connect failed, the connection listens for the
// beacons of Nintendo Zones with the scanner of wds.c for three seconds, and keeps the access point whose signal was
// the strongest. Its steps are a state machine that dwc_connectinet.c polls; the wireless manager's callbacks only
// move the state on.

// How long the scan listens, in milliseconds
#define SCAN_TIME 3000

// Where the scan is. The ones that wait for a callback are named for what is in progress
enum {
    // Nothing started: start the wireless manager
    SCAN_STATE_START,
    SCAN_STATE_STARTING,
    // The wireless manager is up (or the time is not out): start a scan of the channels
    SCAN_STATE_START_SCAN,
    SCAN_STATE_SCANNING,
    // A scan took in its beacons: scan again until the time is out
    SCAN_STATE_SCANNED,
    // The time is out: stop the scan
    SCAN_STATE_STOP_SCAN,
    SCAN_STATE_STOPPING_SCAN,
    // The scan is stopped: read its beacons, and end the wireless manager
    SCAN_STATE_STOPPED,
    SCAN_STATE_ENDING,
    // The wireless manager is down: hand over the strongest access point
    SCAN_STATE_ENDED,
    // Something failed: end the wireless manager
    SCAN_STATE_FAIL,
    // The scan has failed, which every step from here on reports
    SCAN_STATE_FAILED,
};

// What the scan keeps besides the work of wds.c, 0x794 bytes
typedef struct WdsScanData {
    // Where the access point is put
    DWCInetApSettings *settings;
    u16 dmaNo;
    // The tick of the first scan
    u64 startTick;
    WDSEntry entries[WDS_MAX_ENTRIES];
    // The entry with the strongest signal, -1 if there is none
    int best;
} WdsScanData;

// The file's statics
// Black 2's, not checked against Black
typedef struct DWCWdsScan {
    WdsScanData *data;
    // The work of wds.c
    void *wds;
    int state;
} DWCWdsScan;

// pokebw2's sWdsScan
static DWCWdsScan data_02146adc;

static void WDS_Initialize_CB(void *arg);
static void WDS_StartScan_CB(void *arg);
static void WDS_EndScan_CB(void *arg);
static void WDS_End_CB(void *arg);
static void WDS_Error_End_CB(void *arg);

// pokebw2's DWCi_WdsScan_Start
BOOL func_0205d91c(DWCInetApSettings *settings, u16 dmaNo) {
    data_02146adc.wds = DWC_Alloc(DWC_ALLOCTYPE_AC, func_0205a4c8());
    data_02146adc.data = DWC_Alloc(DWC_ALLOCTYPE_AC, sizeof(WdsScanData));
    MI_CpuFill8(data_02146adc.data, 0, sizeof(WdsScanData));
    data_02146adc.data->settings = settings;
    data_02146adc.data->dmaNo = dmaNo;
    data_02146adc.state = SCAN_STATE_START;
    return TRUE;
}

// pokebw2's DWCi_WdsScan_Process
int func_0205d98c(void) {
    int i;
    u16 strongest;
    int result;

    if (data_02146adc.wds == NULL || data_02146adc.data == NULL) {
        return DWC_INET_SCAN_FAILED;
    }
    result = DWC_INET_SCAN_SCANNING;
    switch (data_02146adc.state) {
    case SCAN_STATE_START:
        data_02146adc.state = SCAN_STATE_STARTING;
        if (func_0205a4d4(data_02146adc.wds, WDS_Initialize_CB, data_02146adc.data->dmaNo) != 0) {
            data_02146adc.state = SCAN_STATE_FAILED;
        }
        break;
    case SCAN_STATE_START_SCAN:
        data_02146adc.state = SCAN_STATE_SCANNING;
        if (func_0205a598(WDS_StartScan_CB) == 0) {
            if (data_02146adc.data->startTick == 0) {
                data_02146adc.data->startTick = OS_GetTick();
            }
        } else {
            data_02146adc.state = SCAN_STATE_FAIL;
        }
        break;
    case SCAN_STATE_SCANNED:
        if (OS_TicksToMilliSeconds(OS_GetTick() - data_02146adc.data->startTick) < SCAN_TIME) {
            data_02146adc.state = SCAN_STATE_START_SCAN;
        } else {
            data_02146adc.state = SCAN_STATE_STOP_SCAN;
        }
        break;
    case SCAN_STATE_STOP_SCAN:
        data_02146adc.state = SCAN_STATE_STOPPING_SCAN;
        if (WDS_EndScan(WDS_EndScan_CB) != 0) {
            data_02146adc.state = SCAN_STATE_FAIL;
        }
        break;
    case SCAN_STATE_STOPPED:
        if (func_0205a724(data_02146adc.data->entries) != 0) {
            data_02146adc.state = SCAN_STATE_FAIL;
        }
        // Reads the beacons the scan took in, and picks the one of the strongest signal (the last, among equals) that
        // wds.c takes for a Nintendo Zone's. If they cannot be read, the entries are still the zeros the scan started
        // with, so none is picked; the state set for that failure is overwritten below
        i = 0;
        data_02146adc.data->best = -1;
        strongest = 0;
        for (; i < WDS_MAX_ENTRIES; i++) {
            if (func_0205a8fc(&data_02146adc.data->entries[i].info) && data_02146adc.data->entries[i].valid) {
                if (data_02146adc.data->entries[i].signal >= strongest) {
                    data_02146adc.data->best = i;
                    strongest = data_02146adc.data->entries[i].signal;
                }
            }
        }
        data_02146adc.state = SCAN_STATE_ENDING;
        if (func_0205a558(WDS_End_CB) != 0) {
            data_02146adc.state = SCAN_STATE_FAILED;
        }
        break;
    case SCAN_STATE_ENDED:
        if (data_02146adc.data->best < 0) {
            data_02146adc.state = SCAN_STATE_FAILED;
            break;
        }
        // The wireless manager is down: the strongest access point becomes the settings to connect with, from what its
        // beacon gives
        MI_CpuCopy8(data_02146adc.data->entries[data_02146adc.data->best].info.ssid, data_02146adc.data->settings->ssid,
                    sizeof(data_02146adc.data->settings->ssid));
        MI_CpuCopy8(data_02146adc.data->entries[data_02146adc.data->best].info.wepKey, data_02146adc.data->settings->wepKey,
                    sizeof(data_02146adc.data->settings->wepKey));
        data_02146adc.data->settings->wepMode = data_02146adc.data->entries[data_02146adc.data->best].info.wepMode;
        data_02146adc.data->settings->mtu = data_02146adc.data->entries[data_02146adc.data->best].info.mtu;
        MI_CpuCopy8(data_02146adc.data->entries[data_02146adc.data->best].info.spotInfo, data_02146adc.data->settings->spotInfo,
                    sizeof(data_02146adc.data->settings->spotInfo));
        result = DWC_INET_SCAN_FOUND;
        break;
    case SCAN_STATE_FAIL:
        data_02146adc.state = SCAN_STATE_ENDING;
        if (func_0205a558(WDS_Error_End_CB) != 0) {
            data_02146adc.state = SCAN_STATE_FAILED;
        }
        break;
    case SCAN_STATE_FAILED:
        result = DWC_INET_SCAN_FAILED;
        break;
    }
    return result;
}

// pokebw2's DWCi_WdsScan_Stop
void func_0205dc58(void) {
    if (data_02146adc.state == SCAN_STATE_SCANNED) {
        data_02146adc.state = SCAN_STATE_STOP_SCAN;
    }
}

// pokebw2's DWCi_WdsScan_Free
void func_0205dc74(void) {
    if (data_02146adc.wds != NULL) {
        DWC_Free(DWC_ALLOCTYPE_AC, data_02146adc.wds, 0);
        data_02146adc.wds = NULL;
    }
    if (data_02146adc.data != NULL) {
        DWC_Free(DWC_ALLOCTYPE_AC, data_02146adc.data, 0);
        data_02146adc.data = NULL;
    }
}

// The wireless manager's callbacks: the wireless manager is up, a scan has taken in its beacons, the scan is stopped,
// the wireless manager is ended after a scan, and after a failure
// pokebw2's DWCi_WdsScan_OnInitialized
static void WDS_Initialize_CB(void *arg) {
    data_02146adc.state = SCAN_STATE_START_SCAN;
}

// pokebw2's DWCi_WdsScan_OnScanned
static void WDS_StartScan_CB(void *arg) {
    data_02146adc.state = SCAN_STATE_SCANNED;
}

// pokebw2's DWCi_WdsScan_OnScanStopped
static void WDS_EndScan_CB(void *arg) {
    data_02146adc.state = SCAN_STATE_STOPPED;
}

// pokebw2's DWCi_WdsScan_OnEnded
static void WDS_End_CB(void *arg) {
    data_02146adc.state = SCAN_STATE_ENDED;
}

// pokebw2's DWCi_WdsScan_OnFailed
static void WDS_Error_End_CB(void *arg) {
    data_02146adc.state = SCAN_STATE_FAILED;
}
