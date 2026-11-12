// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/wih.h: its types and constants, and the
// prototypes of Black's named functions.
#ifndef BWPORT_GFL_WIH_H
#define BWPORT_GFL_WIH_H

#include "types.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "nitro/wm.h"

// What the helper calls with the AID of a child that has connected, or of the parent once this machine has connected
typedef void (*WHConnectCallback)(int netId);

// What it calls with the data a machine sent, with the machine's AID, and with no data when a machine has disconnected
typedef BOOL (*WHReceiver)(u16 netId, u8 *data, u16 size);

// What it calls once data has gone out
typedef BOOL (*WHSendCallback)(BOOL ok);

// What the scan calls with each beacon that is a game's, to say whether to go on to connect to it
typedef BOOL (*WHScanCallback)(const void *beacon);

// What the scan calls with each beacon that it finds, whatever it is, with the GFL net work and the signal strength
typedef void (*WHScanInfoCallback)(const void *beacon, void *work, u16 linkLevel);

// How a machine connects (WH_CONNECTMODE_*): in the multi-point mode, with the keys shared, or with the data shared
enum {
    WH_CONNECTMODE_MP_PARENT,
    WH_CONNECTMODE_MP_CHILD,
    WH_CONNECTMODE_KS_PARENT,
    WH_CONNECTMODE_KS_CHILD,
    WH_CONNECTMODE_DS_PARENT,
    WH_CONNECTMODE_DS_CHILD,
};

// pokebw2's WH_ChildConnectAuto
// Connects to a parent as a child, by its MAC address
BOOL func_ov014_02178524(u16 mode, const u8 *mac, u16 channel); // WH_ChildConnectAuto
// pokebw2's WH_StartScan
// Scans for parents, calling back with each beacon it finds
BOOL func_ov014_021785b4(WHScanCallback callback, const u8 *mac, u16 channel); // WH_StartScan
// pokebw2's WH_SetScanTarget
// Sets the channel and MAC address of the parent to connect to
void func_ov014_02178714(u16 channel, const u8 *mac);
// pokebw2's WH_EndScan
BOOL func_ov014_021788fc(void);
// pokebw2's WH_SetGgid
// What net_whpipe.c calls
void func_ov014_02178e1c(u32 ggid);                   // WH_SetGgid
// pokebw2's WH_SetUserGameInfo
void func_ov014_02178e28(const void *data, u16 size); // WH_SetUserGameInfo
// pokebw2's WH_GetBitmap
u16 func_ov014_02178e38(void);                        // WH_GetBitmap
// pokebw2's WH_GetSystemState
int func_ov014_02178e4c(void);  // WH_GetSystemState
// pokebw2's WH_GetLastError
// The wireless error that WH_GetLastError records, and the data the helper received from a machine
int func_ov014_02178e68(void);
// pokebw2's WH_StartMeasureChannel
BOOL func_ov014_02178e84(void); // WH_StartMeasureChannel
// pokebw2's WH_GetMeasureChannel
u16 func_ov014_02179024(void);  // WH_GetMeasureChannel
// pokebw2's WH_Release
// Frees the helper's work
void func_ov014_02179158(void); // WH_Release
// pokebw2's WH_ParentConnect
// Starts as a parent (mode, tgid, channel, most children)
BOOL func_ov014_02179288(u16 mode, u16 tgid, u16 channel, u16 maxEntry);
// pokebw2's WH_SetScanFilter
// How the parent filters the children that connect, and how many frames the scan waits before it starts again
void func_ov014_02179328(NetScanFilter filter);
// pokebw2's WH_SetReceiver
// Sets what the helper calls with the data it receives from a machine
void func_ov014_0217933c(WHReceiver callback); // WH_SetReceiver
// pokebw2's WH_SendData
// Sends data to the parent or the children, then calls back
BOOL func_ov014_02179368(u8 *data, u16 size, WHSendCallback callback); // WH_SendData
// pokebw2's WH_GetSharedDataAdr
u8 *func_ov014_02179394(u16 netId);
// pokebw2's WH_StepDS
BOOL func_ov014_021793b8(u8 *data);
// pokebw2's WH_UpdateScan
void func_ov014_021793f8(void); // Run each frame: starts the scan again
// pokebw2's WH_Finalize
BOOL func_ov014_02179498(void);                // WH_Finalize
// pokebw2's WH_End
BOOL func_ov014_02179570(WHCallback callback); // WH_End
// pokebw2's WH_GetAid
u16 func_ov014_021795a4(void);
// pokebw2's WH_SetScanCallback
// Sets what the helper calls with each beacon the child receives
void func_ov014_021795c0(WHScanCallback callback);
// pokebw2's WH_SetGameInfo
// Sends a beacon of the data, the game's GGID and the TGID
void func_ov014_021795d4(const void *data, int size, u32 ggid, int tgid); // WH_SetGameInfo
// pokebw2's WH_GetBeaconCount
u16 func_ov014_02179614(void);
// pokebw2's WH_SetConnectCallback
// Sets what runs when a machine disconnects
void func_ov014_02179628(WHConnectCallback callback);
// pokebw2's WH_SetScanInfoCallback
// Sets what the scan calls with each beacon that it finds
void func_ov014_0217963c(WHScanInfoCallback callback);
// Whether a beacon the scan found is one of this game
BOOL func_ov014_0217873c(const WMBssDesc *bssDesc);
// pokebw2's WH_SetScanTime
void func_ov014_02179650(u16 time);
// pokebw2's WH_SetScanPaused
// Sets whether the scan stops starting again
void func_ov014_02179664(BOOL paused);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's WH_Initialize
// Starts the helper with its work on the heap given
BOOL func_ov014_021790e4(HeapID heapId, WHCallback callback, u32 extended);
// pokebw2's WH_Reset
void func_ov014_0217945c(void);

// pokebw2's WH_GetSystemState
int func_ov014_02178e4c(void);  // WH_GetSystemState

// pokebw2's WH_SetScanInfoCallback
// Sets what the scan calls with each beacon that it finds

#endif // BWPORT_GFL_WIH_H
