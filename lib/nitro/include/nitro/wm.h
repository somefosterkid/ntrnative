// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/wm.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_NITRO_WM_H
#define BWPORT_NITRO_WM_H

#include "types.h"
#include "struct_decls.h"

// The signal strength, 0 (none) to 3
typedef enum {
    WM_LINK_LEVEL_0,
    WM_LINK_LEVEL_1,
    WM_LINK_LEVEL_2,
    WM_LINK_LEVEL_3,
} WMLinkLevel;

// What the parent's callback gets when a child connects or disconnects
typedef struct {
    u16 apiid;
    u16 errcode;
    u16 wlCmdID;
    u16 wlResult;
    u16 state;
    u8 macAddress[6];
    u16 aid;
    u16 reason;
} WMStartParentCallback;

// The results of the WM functions: they all start an operation and give OPERATING when it has begun, with its result
// to come to the callback
enum {
    WM_ERRCODE_SUCCESS,
    WM_ERRCODE_FAILED,
    WM_ERRCODE_OPERATING,
};

// The state that a callback reports, as the callback's function sees it
enum {
    WM_STATECODE_PARENT_START = 0,
    WM_STATECODE_BEACON_SENT = 2,
    WM_STATECODE_PARENT_NOT_FOUND = 4,
    WM_STATECODE_PARENT_FOUND = 5,
    WM_STATECODE_CONNECTED = 7,
    WM_STATECODE_DISCONNECTED = 9,
    WM_STATECODE_MP_START = 10,
    WM_STATECODE_MPEND_IND = 11,
    WM_STATECODE_MP_IND = 12,
    WM_STATECODE_MPACK_IND = 13,
    WM_STATECODE_PORT_RECV = 0x15,
    WM_STATECODE_DISCONNECTED_FROM_MYSELF = 0x1a,
};

// The first fields of every callback's argument
typedef struct {
    u16 apiid;
    u16 errcode;
} WMCallback;

typedef void (*WMCallbackFunc)(void *arg);

// NitroSDK's enum of results, int-sized
typedef int WMErrCode;

#define WM_SIZE_USER_GAMEINFO 0x70
#define WM_SIZE_WEPKEY 80

// What a parent gives out in its beacon: the game's ID and the user's data. Its size is 0x38
typedef struct {
    u8 *userGameInfo;
    u16 userGameInfoLength;
    u16 padding;
    u32 ggid;
    u16 tgid;
    u16 entryFlag;
    u16 maxEntry;
    u16 multiBootFlag;
    u16 KS_Flag;
    u16 CS_Flag;
    u16 beaconPeriod;
    u8 rsv1[0x18];
    u16 channel;
    u16 parentMaxSize;
    u16 childMaxSize;
} WMParentParam;

// The game's part of a beacon
typedef struct {
    u16 magicNumber;
    u8 ver;
    u8 platform;
    u32 ggid;
    u16 tgid;
    u8 userGameInfoLength;
    u8 gameNameCount_attribute;
    u16 parentMaxSize;
    u16 childMaxSize;
    u8 userGameInfo[0x70];
} WMGameInfo;

// The description of a parent that a scan found. Its size is 0xc0
typedef struct {
    u16 length;
    u16 rssi;
    u8 bssid[6];
    u16 ssidLength;
    u8 ssid[32];
    u16 capaInfo;
    struct {
        u16 basic;
        u16 support;
    } rateSet;
    u16 beaconPeriod;
    u16 dtimPeriod;
    u16 channel;
    u16 cfpPeriod;
    u16 cfpMaxDuration;
    u16 gameInfoLength;
    u16 otherElementCount;
    WMGameInfo gameInfo;
} WMBssDesc;

// The scan's parameters. Its size is 0x44
typedef struct {
    WMBssDesc *scanBuf;
    u16 scanBufSize;
    u16 channelList;
    u16 maxChannelTime;
    u16 bssid[3];
    u16 scanType;
    u16 ssidLength;
    u8 ssid[32];
    u16 ssidMatchLength;
    u16 rsv2[7];
} WMScanExParam;

// The callback of the scan, with the parents it found
typedef struct {
    u16 apiid;
    u16 errcode;
    u16 wlCmdID;
    u16 wlResult;
    u16 state;
    u16 channelList;
    u16 reserved;
    u16 bssDescCount;
    WMBssDesc *bssDesc[16];
    u16 linkLevel[16];
} WMStartScanExCallback;

// What a parent received from one child in a round of the multi-point mode: the length of the data, which is 0xffff if
// the child sent none, the child's AID, and the data
typedef struct {
    u16 length;
    u16 unk_02;
    u16 aid;
    u8 unk_06[4];
    u8 data[];
} WMMpRecvData;

// The buffer that the multi-point mode receives into: the number of children it has room for and the size of the room
// of each
typedef struct {
    u8 unk_00[4];
    u16 count;
    u16 entrySize;
    u8 unk_08[2];
    u8 entries[];
} WMMpRecvBuf;

// The callback of the start of the multi-point mode
typedef struct {
    u16 apiid;
    u16 errcode;
    u16 state;
    u16 reserved;
    WMMpRecvBuf *recvBuf;
} WMStartMPCallback;

// The callback of a disconnection: the AIDs that are gone
typedef struct {
    u16 apiid;
    u16 errcode;
    u16 wlCmdID;
    u16 wlResult;
    u16 tryBitmap;
    u16 disconnectedBitmap;
} WMDisconnectCallback;

// What the indication callback gets
typedef struct {
    u16 apiid;
    u16 errcode;
    u16 state;
    u16 reason;
} WMIndCallback;

// The callback of the child's connection: the parent's answer and the child's AID
typedef struct {
    u16 apiid;
    u16 errcode;
    u16 wlCmdID;
    u16 wlResult;
    u16 state;
    u16 aid;
    u16 reason;
    u16 wlStatus;
    u8 macAddress[6];
    u16 parentSize;
    u16 childSize;
} WMStartConnectCallback;

// The callback of the measure of a channel: how busy it was
typedef struct {
    u16 apiid;
    u16 errcode;
    u16 wlCmdID;
    u16 wlResult;
    u16 channel;
    u16 ccaBusyRatio;
} WMMeasureChannelCallback;

// What a callback of a port gets when a packet came in
typedef struct {
    u16 apiid;
    u16 errcode;
    u16 state;
    u16 port;
    WMMpRecvBuf *recvBuf;
    u8 *data;
    u16 length;
    u16 aid;
} WMPortRecvCallback;

// What the callback of a send of data gets back as its own argument
typedef struct {
    u16 apiid;
    u16 errcode;
    u16 wlCmdID;
    u16 wlResult;
    u16 state;
    u16 port;
    u16 destBitmap;
    u16 restBitmap;
    u16 sentBitmap;
    u16 rsv;
    const u16 *data;
    union {
        u16 size;
        u16 length;
    };
    u16 seqNo;
    WMCallbackFunc callback;
    void *arg;
} WMPortSendCallback;

// The data that a data sharing keeps, and the set that it reads out. Their sizes are 0x820 and 0x200
typedef struct {
    u8 unk_00[0x820];
} WMDataSharingInfo;

typedef struct {
    u8 unk_00[0x200];
} WMDataSet;

WMErrCode WM_SetParentParameter(WMCallbackFunc callback, const WMParentParam *param);              // WM_SetParentParameter
WMErrCode WM_StartParent(WMCallbackFunc callback);                                    // WM_StartParent
WMErrCode WM_Disconnect(WMCallbackFunc callback, u16 aid); // WM_Disconnect
int WM_GetMPReceiveBufferSize(void);                                                       // WM_GetMPReceiveBufferSize
int WM_GetMPSendBufferSize(void);                                                       // WM_GetMPSendBufferSize
// WM_StartMP
WMErrCode WM_StartMP(WMCallbackFunc callback, u16 *recvBuf, u16 recvBufSize, u16 *sendBuf, u16 sendBufSize,
                     u16 mpFreq);
// WM_StartDataSharing
WMErrCode WM_StartDataSharing(WMDataSharingInfo *dsi, u16 port, u16 aidBitmap, u16 dataLength, BOOL doubleMode);
WMErrCode WM_EndMP(WMCallbackFunc callback); // WM_EndMP
WMErrCode WM_EndParent(WMCallbackFunc callback);                                    // WM_EndParent
u16 WM_GetAllowedChannel(void);                                                       // WM_GetAllowedChannel
u16 WM_GetDispersionScanPeriod(void);                                                       // WM_GetDispersionScanPeriod
WMErrCode WM_StartScanEx(WMCallbackFunc callback, const WMScanExParam *param);              // WM_StartScanEx
WMErrCode WM_EndScan(WMCallbackFunc callback);                                    // WM_EndScan
// WM_StartConnectEx
WMErrCode WM_StartConnectEx(WMCallbackFunc callback, const WMBssDesc *bssDesc, const u8 *ssid, BOOL powerSave, u16 authMode);
WMErrCode WM_Reset(WMCallbackFunc callback);                                    // WM_Reset
// WM_SetMPDataToPortEx
WMErrCode WM_SetMPDataToPortEx(WMCallbackFunc callback, void *arg, const u16 *sendData, u16 sendDataSize,
                               u16 destBitmap, u16 port, u16 prio);
// WM_MeasureChannel
WMErrCode WM_MeasureChannel(WMCallbackFunc callback, u16 ccaMode, u16 edThreshold, u16 channel, u16 measureTime);
WMErrCode WM_Initialize(void *wmBuf, WMCallbackFunc callback, u16 dmaNo);            // WM_Initialize
WMErrCode WM_InitializeForListening(void *wmBuf, WMCallbackFunc callback, u16 dmaNo, BOOL extra); // WM_Initialize, with a fourth argument
WMErrCode WM_SetIndCallback(WMCallbackFunc callback);                                    // WM_SetIndCallback
// WM_SetLifeTime
WMErrCode WM_SetLifeTime(WMCallbackFunc callback, u16 tableNumber, u16 camLifeTime, u16 frameLifeTime, u16 mpLifeTime);
u16 WM_GetDispersionBeaconPeriod(void);                                                       // WM_GetDispersionBeaconPeriod
WMErrCode WM_SetPortCallback(u16 port, WMCallbackFunc callback, void *arg);               // WM_SetPortCallback
u16 *WM_GetSharedDataAddress(WMDataSharingInfo *dsi, WMDataSet *dataSet, u16 aid);         // WM_GetSharedDataAddress
WMErrCode WM_StepDataSharing(WMDataSharingInfo *dsi, const u16 *data, WMDataSet *dataSet);              // WM_StepDataSharing
WMErrCode WM_EndDataSharing(WMDataSharingInfo *dsi);                                                    // WM_EndDataSharing
WMErrCode WM_End(WMCallbackFunc callback);                                    // WM_End
// WM_SetGameInfo
WMErrCode WM_SetGameInfo(WMCallbackFunc callback, const u16 *userGameInfo, u16 gameInfoSize, u32 gameId, u16 tgid, u8 attr);

// The next TGID for a parent, counting up from the time (swan names its state tgid_bak)
u16 WM_GetNextTgid(void);

// The next TGID for a parent, counting up from the time (swan names its state tgid_bak)

WMLinkLevel WM_GetLinkLevel(void); // WM_GetLinkLevel

WMErrCode WM_SetBeaconIndication(WMCallbackFunc callback, u16 flag);

// wm_common.c: pokebw2's WM_CheckInitialized, the same as WMi_CheckInitialized
int WM_CheckInitialized(void);

// NitroSDK's static inline functions that Black's headers lacked, as a final ROM compiles them (no assertions).
// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/wm/ARM9/wm_api.h
static inline WMErrCode WM_SetMPDataToPort (WMCallbackFunc callback, const u16 * sendData, u16 sendDataSize, u16 destBitmap, u16 port, u16 prio)
{
	return WM_SetMPDataToPortEx(callback, NULL, sendData, sendDataSize, destBitmap, port, prio);
}
static inline void WM_ConvGgid32to16 (u32 * src, u16 * dst)
{
	dst[0] = (u16)(*src & 0x0000ffff);
	dst[1] = (u16)(*src >> 16);
}
static inline void WM_ConvGgid16to32 (u16 * src, u32 * dst)
{
	*dst = ((u32)src[1] << 16) | src[0];
}
static inline BOOL WM_IsBssidEqual (const u8 * idp1, const u8 * idp2)
{
	return ((*idp1 == *idp2) &&
	        (*(idp1 + 1) == *(idp2 + 1)) &&
	        (*(idp1 + 2) == *(idp2 + 2)) &&
	        (*(idp1 + 3) == *(idp2 + 3)) &&
	        (*(idp1 + 4) == *(idp2 + 4)) && (*(idp1 + 5) == *(idp2 + 5)));
}
static inline void WM_CopyBssid (const u8 * src, u8 * dst)
{
	*dst = *src;
	*(dst + 1) = *(src + 1);
	*(dst + 2) = *(src + 2);
	*(dst + 3) = *(src + 3);
	*(dst + 4) = *(src + 4);
	*(dst + 5) = *(src + 5);
}

// The buffer that the DCF mode receives a frame into: the frame's length, signal, addresses and data
typedef struct WMDcfRecvBuf {
    u16 frameID;
    u16 rsv1[2];
    u16 length;
    u16 rsv2[3];
    u16 rate_rssi; // The signal's strength in the high byte
    u16 rsv3[4];
    u8 destAdrs[6];
    u8 srcAdrs[6];
    u16 rsv4[4];
    u16 data[2];
} WMDcfRecvBuf;

// The callback of the DCF mode: its start (state 14) and each frame that comes in (state 15), in recvBuf
typedef struct {
    u16 apiid;
    u16 errcode;
    u16 state;
    u8 reserved[2];
    WMDcfRecvBuf *recvBuf;
} WMStartDCFCallback;

// What the ARM7 keeps up to date in main memory for the ARM9 to read, after it has invalidated the cache for the part
// that it reads. Its size is 0x7d0. Only the fields that the code of the manager's first files reads are named
typedef struct WMStatus WMStatus;
struct WMStatus {
    u16 state; // WM_STATE_*
    u8 unk_002[0xa];
    BOOL mp_flag;  // TRUE from the start of the multi-point mode to its end
    BOOL dcf_flag; // TRUE from the start of the DCF mode to its end
    u8 unk_014[0x28];
    u16 mp_maxSendSize; // The most that this side sends in a round of the multi-point mode
    u16 mp_maxRecvSize; // The most that the other side sends
    u8 unk_040[0x32];
    u16 mp_recvBufSize; // The size of the part of the ARM7's buffer that holds a round that came in
    u8 unk_074[8];
    void *mp_sendBuf; // The buffer that the multi-point mode sends from
    u8 unk_080[6];
    u16 mp_readyBitmap;
    u8 unk_088[0x14];
    u16 mp_ignoreSizePrecheckMode; // 0 for the multi-point mode to check that its buffers have the sizes that it needs
    u8 unk_09e[0x1e];
    u16 linkLevel; // 0 to 3, as WMLinkLevel
    u8 unk_0be[8];
    u16 pwrMgtMode; // A child may start the multi-point mode only when this is 1
    u8 unk_0c8[0x30];
    u16 maxEntry; // How many children the parent takes
    u8 unk_0fa[0x88];
    u16 child_bitmap; // The AIDs of the children that are connected, one bit each
    u8 pInfoBuf[4];
    u16 aid; // This console's AID, 0 for the parent
    u8 unk_18a[0x646];
};

// wm_system.c: the core that the commands to the ARM7 and their callbacks go through. All of these return a
// WM_ERRCODE_*, WM_ERRCODE_SUCCESS being 0
// WM_Init: starts the manager in the buffer, which must be WM_SYSTEM_BUF_SIZE bytes in main memory, 32-aligned, and
// cleared by the DMA channel dmaNo (0 to 3)
WMErrCode WM_Init(void *wmSysBuf, u16 dmaNo);
// wm_standard.c
WMErrCode WM_Enable(WMCallbackFunc callback);
WMErrCode WM_PowerOff(WMCallbackFunc callback);
WMErrCode WM_EndDCF(WMCallbackFunc callback);
// The status that the ARM7 keeps, NULL before WM_Init
const WMStatus *WMi_GetStatusAddress(void);
// Ends the manager, which must be in the READY state
WMErrCode WM_Finish(void);
WMErrCode WM_Disable(WMCallbackFunc callback);
// WM_SetDCFData: queues a DCF transmission to the six-byte destination address; buffer remains borrowed until
// completion
WMErrCode WM_SetDCFData(WMCallbackFunc callback, const u8 *dst, const u16 *buffer, u16 byteLength);
WMErrCode WM_PowerOn(WMCallbackFunc callback);
// Like WM_SetWEPKey, with the number of the key to use
WMErrCode WM_SetWEPKeyEx(WMCallbackFunc callback, u16 wepMode, u16 wepKeyId, const u8 *wepKey);
// Starts the DCF mode of a child, with the buffer that it receives into
WMErrCode WM_StartDCF(WMCallbackFunc callback, WMDcfRecvBuf *recvBuf, u16 recvBufSize);

#endif // BWPORT_NITRO_WM_H
