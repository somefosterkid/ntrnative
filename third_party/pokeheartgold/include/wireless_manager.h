#ifndef POKEHEARTGOLD_WIRELESS_MANAGER_H
#define POKEHEARTGOLD_WIRELESS_MANAGER_H

#include "global.h"

// The wireless manager of unk_02032844_*. sub_02033528 puts it 32-aligned into the buffer of
// sub_020335B4's size and _021D4124 holds it at +4. The offsets in the comments are the DS ones.

enum WirelessManagerState {
    WIRELESS_STATE_STOP = 0,
    WIRELESS_STATE_IDLE,
    WIRELESS_STATE_SCAN,
    WIRELESS_STATE_BUSY,
    WIRELESS_STATE_CONNECTED,
    WIRELESS_STATE_TRANSMIT_DATA,
    WIRELESS_STATE_TRANSMIT_KEY,
    WIRELESS_STATE_CHECK_CHANNEL,
    WIRELESS_STATE_BAD_CONNECTION,
    WIRELESS_STATE_ERROR,
    WIRELESS_STATE_FATAL_ERROR,
    WIRELESS_STATE_COUNT
};

enum WirelessConnectionType {
    WIRELESS_CONNECTION_MP_SERVER = 0,
    WIRELESS_CONNECTION_MP_CLIENT,
    WIRELESS_CONNECTION_TRANSMIT_KEY_SERVER,
    WIRELESS_CONNECTION_TRANSMIT_KEY_CLIENT,
    WIRELESS_CONNECTION_TRANSMIT_DATA_SERVER,
    WIRELESS_CONNECTION_TRANSMIT_DATA_CLIENT,
    WIRELESS_CONNECTION_COUNT
};

enum ExtendedWMErrCode {
    WM_ERRCODE_DISCONNECTED = WM_ERRCODE_MAX,
    WM_ERRCODE_NO_SERVER,
    WM_ERRCODE_DISCONNECTED_SERVER,
    WM_ERRCODE_NO_CHANNEL,
    WM_ERRCODE_FATAL,
    WM_ERRCODE_GF_MAX
};

typedef void (*WirelessManagerScanFunc)(WMBssDesc *);

typedef void (*WirelessManagerRecvFunc)(u16, u16 *, u16);

typedef void (*WirelessManagerGGIDScanFunc)(u32, s32);

typedef void (*WirelessManagerConnectFunc)(int);

typedef void (*WirelessManagerSendFunc)(BOOL);

typedef struct WirelessManager {
    WMpparam parentParam __attribute__((aligned(32))); // 0x0
    u8 nitroManagerBuffer[WM_SYSTEM_BUF_SIZE] __attribute__((aligned(32))); // 0x40
    u8 sendBuffer[224] __attribute__((aligned(32))); // 0xF40
    u8 recvBuffer[512] __attribute__((aligned(32))); // 0x1020
    WMbssDesc bssDesc __attribute__((aligned(32))); // 0x1220
    WMscanParam scanParam __attribute__((aligned(32))); // 0x12E0
    WirelessManagerScanFunc scanCallback; // 0x1300
    s32 sendBufferSize; // 0x1304
    s32 recvBufferSize; // 0x1308
    u16 channel; // 0x130C
    u16 autoConnect; // 0x130E
    int state; // 0x1310
    int connectionType; // 0x1314
    WirelessManagerRecvFunc recvFunc; // 0x1318
    BOOL (*unusedCallback_1390)(WMStartParentCallback *); // 0x131C
    WirelessManagerGGIDScanFunc ggidScanCallback; // 0x1320
    WirelessManagerConnectFunc disconnectCallback; // 0x1324
    WirelessManagerConnectFunc connectCallback; // 0x1328
    u16 aid; // 0x132C
    u16 connectedBitmap; // 0x132E
    int errorCode; // 0x1330
    u8 numConnectionsMax; // 0x1334
    u8 pauseConnectionClient; // 0x1335
    u32 rand; // 0x1338
    u16 measureChannel; // 0x133C
    u16 measureChannelBusyRatio; // 0x133E
    u16 leastUsedChannelBitmap; // 0x1340
    u8 pauseConnection; // 0x1342
    u8 pauseConnectSystem; // 0x1343
    u8 setEntry; // 0x1344
    u8 sentBeaconCount; // 0x1345
} WirelessManager;

typedef struct WirelessManagerStatics {
    u8 unk_00[4];
    WirelessManager *sWirelessManager; // 0x4
} WirelessManagerStatics;

extern WirelessManagerStatics _021D4124;

void sub_02032844(int state);
void sub_02032858(int errorCode);
BOOL sub_02033528(void *buffer, int connectionType);

#endif // POKEHEARTGOLD_WIRELESS_MANAGER_H
