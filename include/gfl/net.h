// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/net.h, the lines in use so far.
#ifndef BWPORT_GFL_NET_H
#define BWPORT_GFL_NET_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"
#include "gfl/net_handle.h"

struct DWCUserData;
struct DWCFriendData;

// How the game set up the network: what net.c keeps from GFL_NetInit
typedef struct {
    // The game's commands, a NetCommand table
    const void *commandTable;
    int commandNum;
    // Called when a machine disconnects
    void (*disconnectCallback)(void *work, int netId);
    // Called when a machine's negotiation is accepted
    void (*negotiationCallback)(void *work, int netId);
    // The data this machine shares with the others, and its size
    void *(*getInfo)(void *work);
    int (*getInfoSize)(void *work);
    void *(*unk18)(void *work);
    int (*unk1C)(void *work);
    void *unk20;
    void (*unk24)(NetHandle *handle, int a1, void *work);
    void (*unk28)(NetHandle *handle, int a1, void *work);
    void (*unk2C)(void *work);
    void (*unk30)(void *work);
    // 12 bytes that the games' init data zero, the last four of which are a function that dwc_rap.c calls with each
    // Wi-Fi library error it queues (its code and its second value), and the game's work
    union {
        u8 unk34[0xc];
        struct {
            u8 unk34_0[8];
            void (*callback)(int code, int param, void *work);
        } wifiError;
    } unk34;
    // The Wi-Fi friend list and the player's user data, from the save's WifiList
    struct DWCFriendData *(*getFriendData)(void *work);
    struct DWCUserData *(*getUserData)(void *work);
    // The size of the heap for Wi-Fi
    u32 wifiHeapSize;
    // The game's ID in the wireless beacons (gameId, of which the low 16 bits go in them) after four more bytes. The
    // games' init data give the bytes
    union {
        u8 unk4C[8];
        struct {
            u32 unk4C_0;
            u32 gameId;
        } id;
    } unk4C;
    HeapID parentHeapId;
    HeapID heapId;
    HeapID wifiHeapId;
    HeapID ircHeapId;
    // Where the wireless signal icon is
    u16 iconX;
    u16 iconY;
    u8 maxConnectNum;
    // The largest packet
    u8 maxSendSize;
    u8 unk62;
    u8 unk63;
    // Whether the parent relays every machine's data (MP mode)
    u8 bMPMode;
    // The kind of connection: 1, 2 and 6 are Wi-Fi
    u8 bNetType;
    u8 unk66;
    // The high byte of the game's commands
    u8 gameCommandBase;
    union {
        u8 bytes[4];
        u32 word;
    } unk68;
    u16 unk6C;
    u16 unk6E;
} GFLNetInitData;

// The most machines that can connect
#define GFL_NET_MACHINE_MAX 8

// What the network device does, wireless or Wi-Fi; the code calls each through GFLNetSys
typedef BOOL (*GFLNetRecvFunc)(u16 netId, u8 *data, u16 size);
typedef BOOL (*GFLNetSendDoneFunc)(BOOL ok);

typedef struct {
    void (*unk00)(int a0, int a1);
    // Starts the device; the callback, which the wireless device calls when its helper ends, is NULL from the net
    BOOL (*init)(HeapID heapId, void *sys, BOOL (*callback)(BOOL success), void *work);
    BOOL (*unk08)(void (*callback)(BOOL ok));
    // Runs the device each frame with the connected machines, returning a negative error or a status
    int (*update)(u16 connectBits);
    BOOL (*unk10)(int a0, int a1);
    BOOL (*unk14)(void (*callback)(BOOL ok));
    BOOL (*unk18)(void);
    BOOL (*unk1C)(void);
    BOOL (*unk20)(void);
    BOOL (*unk24)(void);
    // A found beacon's data and MAC address, by index
    void *(*unk28)(int index);
    u8 *(*unk2C)(int index);
    BOOL (*unk30)(void);
    BOOL (*unk34)(void);
    BOOL (*unk38)(int a0);
    BOOL (*setDisconnectCallback)(void (*callback)(int netId));
    int (*unk40)(int a0, BOOL (*callback)(void));
    BOOL (*unk44)(int a0, int a1);
    int (*unk48)(BOOL a0, const u8 *mac, int a2, int a3, void (*callback)(void));
    BOOL (*unk4C)(const char *key, int numEntry, BOOL a2, int a3);
    BOOL (*unk50)(int a0, int a1, int a2);
    BOOL (*unk54)(BOOL a0, int a1);
    BOOL (*unk58)(void);
    BOOL (*unk5C)(u8 *data);
    u8 *(*getRecvData)(int netId);
    BOOL (*send)(u8 *data, int size, int a2, GFLNetSendDoneFunc callback);
    BOOL (*setRecvCallback)(GFLNetRecvFunc callback);
    BOOL (*unk6C)(void);
    BOOL (*isConnected)(void);
    BOOL (*unk74)(void);
    BOOL (*unk78)(void);
    int (*getConnectBits)(void);
    int (*getNetId)(void);
    int (*getSignalLevel)(void);
    int (*isError)(void);
    void (*unk8C)(int a0);
    int (*unk90)(int a0);
    int (*unk94)(void);
    int (*unk98)(void);
    void (*unk9C)(void);
    BOOL (*unkA0)(void);
    BOOL (*unkA4)(void);
    BOOL (*unkA8)(void);
    BOOL (*unkAC)(void);
    BOOL (*unkB0)(void);
    void (*unkB4)(void);
    void (*unkB8)(int a0);
    BOOL (*unkBC)(void);
    void (*unkC0)(int a0);
    void (*unkC4)(int a0);
    void (*unkC8)(void *a0);
} GFLNetDevTable;

// The network library's state, with a copy of the game's init data
typedef struct {
    GFLNetInitData aNetInit;
    NetHandle handles[GFL_NET_HANDLE_MAX];
    const GFLNetDevTable *pDevTable;
    u8 unk344[4];
    // Called when the network has ended
    void (*exitCallback)(void *work);
    void *devWork;
    u8 unk350[2];
    u8 unk352;
    u8 unk353;
} GFLNetSys;

// The network error, which func_0203c9ac records
typedef struct {
    int code;
    u32 unk4;
    u32 unk8;
    // Nonzero once an error is recorded, the line it was found on for net_system.c's
    int type;
} GFLNetErrorInfo;

void func_0203acac(void);
// Command handlers of the other parts of the library
void func_0203a770(int netId, int sender, int command, int size, void *data, NetHandle *handle);
BOOL func_0203a7ec(int command);
BOOL func_0203a804(int command);
void *func_0203a81c(int command, int netId, int size);
BOOL func_0203a634(int netId);

// pokebw2's GFL_NetErrCheck and GFL_NetErrMarkShown
BOOL func_0200fb24(void);
void func_0200fb38(void);
BOOL GFL_NetErrShow(u32 a0);
// Records the offset of BG 1 of the main engine, which bg_sys.c sets whenever it changes. pokebw2's func_02042ee0
void func_0203d814(int x, int y);

// net.c: starts and ends the network, and passes calls to the device and the other parts of the library
void func_0203cbac(const GFLNetInitData *pNetInit, void (*callback)(void *work), void *work);
BOOL func_0203cd98(void);
// pokebw2's func_020427a4
BOOL func_0203cdbc(void);
// Ends the network, calling back with the game's work once it has
BOOL func_0203ceac(void (*callback)(void *work));
void *func_0203cf10(int index);
u8 *func_0203cf3c(int index);
void func_0203cff0(const u8 *mac);
// Starts as a parent, with a callback for when the connection is up, or NULL
void func_0203d10c(void (*callback)(void *work));
void func_0203d000(const u8 *mac);
void func_0203d010(void);
void func_0203d01c(void);
void func_0203d0dc(void (*callback)(void *work));
void func_0203d17c(int a0);
u8 func_0203d1a4(NetHandle *handle);
BOOL func_0203d210(void);
// Send a command to every machine; the game's commands wait for the negotiation
BOOL func_0203d3a8(NetHandle *handle, int command, u16 size, const void *data);
BOOL func_0203d4d0(NetHandle *handle, u8 dest, u16 command, int size, const void *data, u32 a5, BOOL a6, BOOL noCopy);
int func_0203d680(void);
int func_0203d6bc(void);
GFLNetSys *func_0203d788(void);
GFLNetInitData *func_0203d798(void);
// pokebw2's func_02042e94 and func_02042e9c
void func_0203d7b0(u8 a0);
void func_0203d7bc(BOOL a0);
// Sends a command to every machine, or to one (pokebw2's func_02042c18)
BOOL func_0203d3f8(NetHandle *handle, u8 sendID, u16 command, u32 size, const void *data, u32 a5, BOOL a6,
                   BOOL noCopy);

// The kinds of connection: Wi-Fi through DWC, and the others through the wireless or infrared devices
#define GFL_NET_TYPE_WIFI 1

#define GFL_NET_TYPE_WIFI_LOBBY 2
#define GFL_NET_TYPE_WIFI_GTS 6
typedef BOOL (*NetScanFilter)(const void *info, void *work);

// The helper's states (WH_SYSSTATE_*)
enum {
    WH_SYSSTATE_STOP,
    WH_SYSSTATE_IDLE,
    WH_SYSSTATE_SCANNING,
    WH_SYSSTATE_BUSY,
    WH_SYSSTATE_CONNECTED,
    WH_SYSSTATE_DATASHARING,
    WH_SYSSTATE_KEYSHARING,
    WH_SYSSTATE_MEASURECHANNEL,
    WH_SYSSTATE_CONNECT_FAIL,
    WH_SYSSTATE_ERROR,
    WH_SYSSTATE_FATAL,
};

// Called when the wireless has ended, with whether that succeeded. The callers ignore its result
typedef BOOL (*WHCallback)(BOOL success);

void *allocConfigDSSoftwareFeature(HeapID heapId, u32 size, const char *file, u16 line);

u8 func_0203d5d4(void);

BOOL func_0203d29c(void);
void func_0200f698(u16 trackMask, s32 volume);

// Whether the error was handled, after shutting the connection down
BOOL func_0200fbb0(void);
// Calls into the functions that show the wireless strength icons
void func_0203d34c(BOOL top, HeapID heapId);

// The game's work, which the callbacks get
void *func_0203d650(void);
void func_0203d7fc(void *ptr);
void func_02010864(u8 linkLevel);

void func_0203d720(int a0);
void func_0203d754(int a0);

// net_err.c: the network error screen. The error's code is its message in TEXT_BANK_WIFI_ERROR
// Creates the network's heap, and the buffers the error screen saves the display in
void GFL_NetHeapInit(void);
// Shows the error screen for an assertion that failed
void AssertFailErrorDisp(void);
void func_0203d864(void);
// Steps the network while the game waits for it, as before a soft reset
BOOL func_0203cf5c(void);
void func_0203cfa0(void);
void func_0203d828(int *x, int *y);
// Finds the network's error, each frame
void func_0200f734(void);
// Shows the fatal error screen for a Wi-Fi error with the message given
void func_0200f7c0(u32 message);
void func_0200f7e8(BOOL keepDark);
void func_0200f800(void);
// Shows the error screen until A or B is pressed, with the wireless icon
void func_0200f83c(void);
void func_0200faac(void);
// Records a Wi-Fi error with the message given, to show it
void func_0200fb4c(u32 message);
void func_0200fba0(void);

BOOL func_0203d378(void);
// Whether a machine's negotiation has been accepted
BOOL func_0203d394(NetHandle *handle);
// The game's work, which the callbacks get

// The wireless signal level of the DS Download Play, and the record of the best level a scan saw, which it sets and
// clears
u16 func_02010848(void);
void func_02010498(void);

// Steps the network while the game waits for it, as before a soft reset
// Whether the net game command base is one of the battle kinds
BOOL func_0200f300(void);

// The game's work, which the callbacks get
void func_0203d700(void);
// Steps the network while the game waits for it, as before a soft reset

// Shows the error screen until A or B is pressed, with the wireless icon

// net.c: starts and ends the network, and passes calls to the device and the other parts of the library
void func_0203cb34(int a0, int a1, HeapID parentHeapId, HeapID heapId);
void func_0203d060(void (*callback)(void *work), void (*a1)(void *work, BOOL a1), void (*a2)(void *work));
BOOL func_0203d1c0(u8 netId);
void func_0203d1e8(NetHandle *handle, int a1);
// Whether the network is infrared, or Wi-Fi
BOOL func_0203d26c(void);
// The second argument is unused
void func_0203d5ac(u8 gameCommandBase, u8 unused);
// The game's Wi-Fi user data and friend list, from its init data's getUserData and getFriendData
struct DWCUserData *func_0203d5ec(void);
struct DWCFriendData *func_0203d610(void);
u8 func_0203d634(void);
void func_0203d668(void *work);
void func_0203d844(const GFLNetInitData *pNetInit);
void func_0203d88c(int x, int y);
void func_0203d8ac(void);
void func_0203d8c4(BOOL a0);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void GFL_NetErrAbort(void);
u16 func_0203d6cc(void);
void GFL_NetErrInit(u32 heapId);
BOOL func_0203d880(void);
void func_0203cf04(void);
void func_0203d0d0(void);
void func_0200f7f4(void);
BOOL func_0205d074(void);
// The state of the wireless connections, as bits: 0x2 a local wireless one, 0x3c the signal, 0x3c0 Wi-Fi
u32 func_0201058c(WifiList *wifiList);
int func_0203d1b4(void);
void func_0203d14c(int mode, int a1, const u8 *mac);
void func_02010884(void);
void *func_02010360(int num, HeapID heapId);
void func_020103e4(void *work);
// The device table for a GFL_NET_TYPE_*, from outside the library
const GFLNetDevTable *func_0200f17c(int type);
void func_0200f234(int type);
void func_0203d0ac(int a0);
void func_0203d0b8(u8 *mac, int index);
void func_0203d0c4(void *a0);
void func_0203d644(BOOL a0);
BOOL func_0203d56c(NetHandle *handle);
// The state of the wireless connections, as bits: 0x2 a local wireless one, 0x3c the signal, 0x3c0 Wi-Fi
// GFL_NetErrShow(1)
void func_0200f7dc(void);
void func_0205cc20(void);
// GFL_NetErrShow(1)
u8 func_0203d170(void);
void func_0203d164(int a0);
BOOL func_0203d158(void);
// Whether a command is still being sent
BOOL func_0203d578(NetHandle *handle, u16 command);
void func_0203d594(NetHandle *handle, u16 timing);
BOOL func_0203d5a0(NetHandle *handle, u16 timing);
void func_0203d0fc(int a0); // the argument is unused
int func_0205ccd8(void);
// The argument is unused

void func_0203d12c(HeapID heapId);

#endif // BWPORT_GFL_NET_H
