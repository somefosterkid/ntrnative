// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/bm.h: its types and constants,
// and the prototypes of Black's named functions.
#ifndef BWPORT_DWC_BM_H
#define BWPORT_DWC_BM_H

#include "types.h"

#define DWC_BM_PACKED_SIZE 14

// What the settings keep of one connection (a slot of the console's Wi-Fi settings): the network name, the WEP keys,
// the address settings, the WEP mode, the kind of the connection (DWC_SETTYPE_NONE for a slot not in use; 0 to 2 are
// the kinds that an NTR console has, 1 being AOSS, more are the TWL's and 0x10 to 0x13 use WPA), its MTU, and in the
// first two pages the mask of the connections in use (bit n is set while connection n is). The first 0xF0 bytes of a
// page
#define DWC_SETTYPE_NONE 0xFF
typedef struct DWCBMApInfo {
    u8 unk0[0x20];     // the provider's ID
    u8 unk20[0x20];    // and its password
    u8 ssid[0x20];     // 0x40
    u8 aossSsid[0x20]; // 0x60; the SSID of the 40-bit WEP key of an AOSS setup, which has an SSID for each kind of key
    u8 wepKey[4][0x10]; // 0x80; the first 5, 13 or 16 bytes of the first, as wepMode says
    u8 ip[4];          // 0xc0; all 0 when the address comes from DHCP
    u8 gateway[4];     // 0xc4
    u8 dns[2][4];      // 0xc8; the first is all 0 when the DNS servers come from DHCP
    u8 netmask;        // 0xd0; how many of the mask's high bits are set, 0x20 at most
    u8 wep2[4][5];     // 0xd1; the 40-bit WEP keys of an AOSS setup
    u8 unkE5;
    u8 wepMode : 2;     // 0xe6; 0 for no WEP, 1 for a key of 5 bytes, 2 of 13 and 3 of 16
    u8 wepNotation : 6; // 1 when the key was entered as characters, 0 when as hex digits
    u8 setType;         // 0xe7; DWC_SETTYPE_NONE or the kind
    u8 unkE8[2];
    u16 mtu; // 0xea; 0 for the default
    u8 unkEC[3];
    u8 state; // 0xef; the mask of the connections in use, in the first two pages
} DWCBMApInfo;

// A page of the NTR console's Wi-Fi settings, 0x100 bytes: a connection, the packed Wi-Fi ID, and the CRC16 of all that
// comes before it
typedef struct DWCMemPage {
    DWCBMApInfo ap;
    u8 wifi[DWC_BM_PACKED_SIZE]; // 0xf0
    u16 crc;                     // 0xfe
} DWCMemPage;

// The second half of a page of the TWL console's settings, from 0x100 of the page: the WPA keys and the SSIDs that go
// with them, the WPA mode, the proxy, and the CRC16 of this half of its own
typedef struct DWCBmTwlExtra {
    u8 wpaKey[0x20];  // 0x100; the key for the WCM mode that wcmMode says, and for the mode 4
    u8 wpaKey2[0x20]; // 0x120; the key for the mode 6
    u8 ssid3[32];     // 0x140; the SSID that goes with the key of wpaKey
    u8 ssid4[32];     // 0x160; and the one that goes with wpaKey2
    u8 unk_80;
    u8 wcmMode; // 0x181; the mode that func_ov009_021652a4 wants of the setting's WPA
    u8 unk_82;  // 0x182; 0 or 1: the proxy flag, by the layout of a DSi's settings (no connection test for a proxy)
    u8 unk_83;  // 0x183; 0 or 1
    u8 unk_84[0x64];
    u16 unk_e8;
    u8 unk_ea[0x14];
    u16 crc; // 0x1fe; of all of this half that comes before it
} DWCBmTwlExtra;

// A page of the TWL console's settings, 0x200 bytes (the DSi keeps three, from page 0x11 of the NVRAM): a first half
// laid out like an NTR page, whose kinds of connection go on to those of WPA (from 0x10), and the extra half.
// Auto-connect reads them into its search's work (dwc/ac.h's DWCAcSearchWork.dsiSettings), dwc_bm_check.c checks them
typedef struct DWCBmTwlPage {
    DWCMemPage ntr;
    DWCBmTwlExtra extra; // 0x100
} DWCBmTwlPage;

typedef struct DWCBmWifiIdInfo {
    u64 id;        // 43 bits; 0 while no ID has been confirmed
    u64 pendingId; // 43 bits; made from the console's MAC address, the time and the salt
    u16 number;    // 10 bits; a random number from 0 to 999
    u16 salt;      // 16 bits; part of what the pending ID is made from
} DWCBmWifiIdInfo;

// What the Wi-Fi ID is given out as: the two IDs, and whether the first is set
typedef struct DWCBmWifiId {
    u64 id;
    u64 pendingId;
    u32 isSet;
} DWCBmWifiId;

// What the checks of dwc_bm_check.c return: 0 when the settings are whole, or were made whole again without losing a
// connection, or a negative number for what failed or was lost. DWC_InitForProduction takes any of them as the settings
// being destroyed
typedef enum DWCBmCheckResult {
    DWC_BM_CHECK_WRITE_FAILED = -10000,    // the repaired settings couldn't be written
    DWC_BM_CHECK_READ_FAILED = -10001,     // the NVRAM couldn't be read
    DWC_BM_CHECK_CONNECTION_LOST = -10002, // a connection that was in use is gone
    DWC_BM_CHECK_WIFI_ID_LOST = -10003,    // the first two pages, which both keep the Wi-Fi ID, are gone
} DWCBmCheckResult;

// dwc_bm_check.c. Reads the four pages of the NTR settings (wih_dwc.c's local configuration, at +0x600 of its 0xa00
// bytes; Black 2's also reads the TWL's at +0). The second argument is not used (the caller passes 0)
typedef enum DWCBmReadResult {
    DWC_BM_READ_FAILED = -1,
    DWC_BM_READ_OK,
} DWCBmReadResult;

DWCBmReadResult func_0205faa8(DWCMemPage *pages, int unused);

// dwc_bm_init.c, DWC_BM_Init. The same checks for the console the game runs on, then the Wi-Fi ID that the settings
// hold is kept. The work area is 0x1000 bytes. Returns the result of the checks, or -10001 when the four pages of the
// NTR console's settings can't be read
// The size of the work area that DWC_BM_Init is given and clears
#define DWC_BM_WORK_SIZE 0x1000

u32 DWC_BM_Init(void *param_1);

#endif // BWPORT_DWC_BM_H
