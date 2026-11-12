// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/wds.h, with Black's names.
#ifndef BWPORT_DWC_WDS_H
#define BWPORT_DWC_WDS_H

#include "types.h"
#include "nitro/crypto/rc4.h"
#include "nitro/math.h"
#include "nitro/wm.h"

// A beacon scanner of DWC (TWL-DWC 5.3, ARM9 main; wds.c), on NitroSDK's wireless manager. It scans for the wireless
// parents whose beacons carry the game ID 0x857 and a block of 0x70 bytes of game information, encrypted with RC4
// under a key made from the text "WDS!" and four bytes of the beacon's BSSID, and checked with a CRC-16. The ROM names
// none of it; the names, the WDS_ functions' included, are ours, taken from what the code does.
//
// The caller gives a work area of func_0205a4c8() bytes to func_0205a4d4 (WM_Initialize), starts a scan with
// func_0205a598, and when the scan's callback comes reads the beacons it found with func_0205a724. WDS_EndScan stops
// the scan and func_0205a558 ends the wireless manager.

// What a beacon says, once it is decrypted: 0x70 bytes. func_0205a8fc checks three of them, and the last two
// are the CRC-16 of the rest (a beacon that carries 0 there is taken as it is)
typedef struct WDSBeaconInfo {
    u8 ssid[0x20];
    u8 spotInfo[10]; // ten printable characters; the connection code reads the first as a digit
    u8 unk_2a[0x1a];
    u8 wepKey[0x20];
    u8 unk_64;
    u8 wepMode; // at most 3
    u8 flags;   // bit 1 must be set
    u8 unk_67[5];
    u16 mtu;
    u16 crc;
} WDSBeaconInfo;

// A beacon that the scan found, as func_0205a724 gives it out: whether the entry is filled, the strength of its
// signal and what the beacon says
typedef struct WDSEntry {
    int valid;
    u16 signal;
    WDSBeaconInfo info;
} WDSEntry;

#define WDS_MAX_ENTRIES 16

// The size of the work area that func_0205a4d4 takes, a multiple of the cache line of 32 bytes
u32 func_0205a4c8(void);
// Sets up the work area and starts the wireless manager (WM_Initialize); 0 if it began, and the callback gets the rest.
// A callback of NULL gives -1
int func_0205a4d4(void *work, WMCallbackFunc callback, u16 dmaNo);
// Ends the wireless manager (WM_End) and lets go of the work area; the callback gets the result
int func_0205a558(WMCallbackFunc callback);
// Starts a scan of the allowed channels (WM_StartScanEx); the callback gets the scan's result once the beacons are
// taken in
int func_0205a598(WMCallbackFunc callback);
// Stops the scan (WM_EndScan)
int WDS_EndScan(WMCallbackFunc callback);
// Clears WDS_MAX_ENTRIES entries and fills them with the beacons that the scan took in, in the order it did;
// -1 if there is no finished scan
int func_0205a724(WDSEntry *entries);
// Decrypts the information of a beacon into `out` and checks its CRC-16. The caller gives the cipher's state, a
// CRC-16 accumulator and its table (connectinet.c has them in its own work area). FALSE if the beacon fails, with
// `out` cleared
BOOL func_0205a7cc(CRYPTORC4Context *context, u16 *crc, MATHCRC16Table *table, const WMBssDesc *desc,
                           WDSBeaconInfo *out);
// Whether the decrypted information of a beacon is well-formed
BOOL func_0205a8fc(const WDSBeaconInfo *info);

#endif // BWPORT_DWC_WDS_H
