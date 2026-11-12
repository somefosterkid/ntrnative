// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2) at 9a3236bc8, lib/nitrowifi/include/nitrowifi/wcm.h.
#ifndef BWPORT_NITROWIFI_WCM_H
#define BWPORT_NITROWIFI_WCM_H

#include "types.h"
#include "nitro/wm.h"
#include "nitro/os.h"

// NitroWiFi's wireless connection manager (WCM), in overlay 9, which DWC's settings in overlay 16 drive. The
// ROM names none of it: the names are those of the SDK's WCM calls that match what the functions do and where the
// settings call them.

// A word of a WCM notification, which carries a number or a pointer
typedef union {
    s32 n;
    void *p;
} WCMNoticeParameter;

// What a WCM notification reports: the kind of the operation that ended, its result and three words about it (for a
// found access point, the first is its WMBssDesc; for a connection, the second is why it failed)
typedef struct {
    s16 notify;
    s16 result;
    WCMNoticeParameter parameter[3];
} WCMNotice;

// notify
enum {
    WCM_NOTIFY_COMMON,
    WCM_NOTIFY_STARTUP,
    WCM_NOTIFY_CLEANUP,
    WCM_NOTIFY_BEGIN_SEARCH,
    WCM_NOTIFY_END_SEARCH,
    WCM_NOTIFY_CONNECT,
    WCM_NOTIFY_DISCONNECT,
    WCM_NOTIFY_FOUND_AP,
};

// What the calls answer; the call was taken, and the notification follows
#define WCM_RESULT_ACCEPT 3

// A func_ov009_02164d64's work, and the configuration that func_ov009_02164f98 starts with
#define WCM_WORK_SIZE 0x2300
#define WCM_APLIST_SIZE 192

typedef struct {
    s32 dmano;
    void *pbdbuffer;
    s32 nbdbuffer;
    s32 nbdmode;
} WCMConfig;

typedef void (*WCMNotify)(WCMNotice *notice);

// The key func_ov009_021652a4 is given: a WEP mode (1 to 3) with its key index, or a WPA mode (4 to 7) whose keyId is
// the length of the passphrase in key; mode 0 for an open network
typedef struct WCMWepDesc {
    u8 mode;
    u8 keyId;
    u8 key[0x50]; // WM_SIZE_WEPKEY
} WCMWepDesc;

// The options of func_ov009_021652a4 that pick the power saving and the authentication (descriptive names). Each is a
// pair of bits: the upper one says that the option is given, the lower one is its value (func_ov009_021656d0 sets or
// clears 0x10000 and 0x40000 by it), and the connection reads them back as "!= 0x30000" for power saving and
// "== 0xc0000" for a shared key
#define WCM_OPTION_POWER_SAVE 0x20000
#define WCM_OPTION_POWER_ACTIVE 0x30000
#define WCM_OPTION_AUTH_OPEN 0x80000
#define WCM_OPTION_AUTH_SHARED 0xc0000



// The "any" BSSID and ESSID that a search takes when the caller gives none (NitroWiFi's names; pokebw2's
// WCMiAnyBssid and WCMiAnyEssid)
extern const u8 WCM_Bssid_Any[];
extern const u8 WCM_Essid_Any[];

// Black 2's, not checked against Black
#define WCM_OPTION_AUTH_SHAREDKEY 0xc0000

// Black 2's, not checked against Black
// The function that every received data frame goes to
typedef void (*WCMRecvInd)(const u8 *srcAddr, const u8 *dstAddr, const u8 *buf, s32 len);

// pokebw2's WCM_GetLinkLevel
// Returns a computed level from 0 through 3 in connected state 9, or zero otherwise.
WMLinkLevel func_ov009_021666c8(void);

// pokebw2's WCMi_EntryApList
// A beacon or probe response NitroSDK's WM handed WCM; fills or touches its WCM_PointApList entry
void func_ov009_02165d04(const WMBssDesc *bss, u16 scanLinkLevel);
// pokebw2's WCMi_CpsifRecvCallback
void func_ov009_02166358(const u8 *src, const u8 *dst, const u8 *data, s32 len);
// pokebw2's WCM_UpdateOption
// Updates the selected option fields and ORs the entire normalized incoming word, returning the previous word.
u32 func_ov009_021656d0(u32 option);
// pokebw2's WCMi_CpsifUnlockMutexInIRQ
// Releases one mutex level only when its owner is the IRQ-handler address used as a special marker.
void func_ov009_02166594(OSMutex *mutex);

// pokebw2's WCM_GetApMacAddress
// The MAC address of the access point the connection is with, or NULL while there is none; NitroWiFi's SOCL takes
// the link to be up while there is one
u8 *func_ov009_02166384(void);
// pokebw2's WCM_GetApEssid
// Borrowed SSID bytes and optional length, or NULL/zero while disconnected. The bytes need not be
// NUL-terminated and remain owned by WCM; a connection change or Finish can invalidate the pointer.
u8 *func_ov009_021663c8(u16 *ssidLength);
// pokebw2's WCM_SetRecvDCFCallback
// Sets the function that every received data frame goes to
void func_ov009_02166424(WCMRecvInd callback);
// pokebw2's WCM_SendDCFDataEx
// Sends to the six-byte address dst, copying headerLen/dataLen bytes into WCM's work during this call. Assembly
// precedes backend length validation. Returns -1 without work or -4 for an invalid state/reset in progress;
// after a send attempt it returns headerLen + dataLen even if the backend rejected the assembled frame.
s32 func_ov009_02166444(const u8 *dst, const u8 *header, s32 headerLen, const u8 *data, s32 dataLen);
// pokebw2's WCM_GetMacAddress
// Copies the MAC address of the wireless hardware to mac
void func_ov009_021665e4(u8 *mac);

// pokebw2's WCM_ConnectAsync
// Accepted descriptors are copied into WCM's work during this call; their pointers are not retained.
// option is applied through WCM_UpdateOption before the connection backend starts.
s32 func_ov009_021652a4(void *bssDesc, void *wepDesc, u32 option);
// pokebw2's WCM_DisconnectAsync
s32 func_ov009_02165478(void);
// pokebw2's WCM_TerminateAsync
// Restores idle operation through the selected backend, resetting an active connection if needed.
s32 func_ov009_02165560(void);
// pokebw2's WCM_GetPhase
s32 func_ov009_021656a0(void);
// pokebw2's WCM_GetAllowedChannel
// DS mode fills gaps between the lowest and highest hardware-allowed channel bits; DSi uses its backend mask.
// Returns zero outside states 3 through 10.
s32 func_ov009_0216576c(void);
// pokebw2's WCM_Init
s32 func_ov009_02164d64(void *work, s32 size);
// pokebw2's WCM_InitEx
// WCM_Init selects mode 1 (DS). The core also accepts 2 (DSi, requiring 0x4800 bytes), or 0 to use the stored
// preference or hardware default. Main's direct caller selects mode 2; this parameter is a mode, not a boolean.
s32 func_ov009_02164d74(void *work, s32 size, s32 requestedMode);
// pokebw2's WCM_Finish
// Releases the work buffer: 0, or 1 when WCM is not initialized or not idle (state 1)
s32 func_ov009_02164ee0(void);
// pokebw2's WCM_StartupAsync
s32 func_ov009_02164f98(WCMConfig *config, WCMNotify notify);
// pokebw2's WCM_CleanupAsync
s32 func_ov009_02165088(void);
// pokebw2's WCM_SearchAsync
// Either NULL pointer requests a stop; option is ignored on that path.
s32 func_ov009_02165144(void *bssid, void *essid, u32 option);
// pokebw2's WCM_EndSearchAsync
// Requests a scan stop, with completion through WCM_NOTIFY_END_SEARCH.
s32 func_ov009_02165204(void);

// pokebw2's WCM_ClearApList
// Clears the AP-list storage with IRQs disabled, invalidating borrowed AP-list records regardless of list lock.
void func_ov009_02165b80(void);
// pokebw2's WCM_CountApList
s32 func_ov009_02165bd0(void);
// pokebw2's WCM_LockApList
s32 func_ov009_02165c28(s32 lock);
// pokebw2's WCM_PointApList
WMBssDesc *func_ov009_02165ca0(u16 index);
// pokebw2's WCMi_BssDescCopy
// destination is a raw 0xc0-byte retained descriptor with selected IEs packed after offset 0x40.
void func_ov009_02166040(const WMBssDesc *source, WMBssDesc *destination);

#endif // BWPORT_NITROWIFI_WCM_H
