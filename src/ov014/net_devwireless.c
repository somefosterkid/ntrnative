// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov030/net_devwireless.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "gfl/net_devwireless.h"
#include "types.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_whpipe.h"
#include "gfl/wih.h"
#include "nitro/wm.h"

// The 8-byte work that the device keeps while it runs
typedef struct {
    void *sys;
    // Called when the wireless has ended
    void (*endCallback)(BOOL ok);
} NetDevWireless;

static BOOL func_ov014_02177028(HeapID heapId, void *sys, WHCallback callback, void *work);
static BOOL func_ov014_02177080(void (*callback)(BOOL ok));
static int func_ov014_02177094(u16 connectBits);
static BOOL func_ov014_021770a0(int a0, int a1);
static BOOL func_ov014_021770cc(void (*endCallback)(BOOL ok));
static BOOL func_ov014_021770ec(void);
static BOOL func_ov014_021770f8(void);
static BOOL func_ov014_021770fc(void);
static BOOL func_ov014_02177100(void);
static void *func_ov014_02177104(int index);
static u8 *func_ov014_02177114(int index);
static BOOL func_ov014_02177124(void);
static BOOL func_ov014_02177128(void);
static BOOL func_ov014_0217712c(int a0);
static BOOL func_ov014_02177134(void (*callback)(int netId));
static int func_ov014_02177140(int a0, BOOL (*callback)(void));
static BOOL func_ov014_02177148(int a0, int a1);
static int func_ov014_02177150(BOOL a0, const u8 *mac, int index, int a3, void (*callback)(void));
static BOOL func_ov014_0217715c(const char *key, int numEntry, BOOL a2, int a3);
static BOOL func_ov014_02177160(BOOL a0, int a1);
static BOOL func_ov014_02177168(void);
static BOOL func_ov014_0217717c(u8 *data);
static u8 *func_ov014_02177184(int netId);
static BOOL func_ov014_02177190(u8 *data, int size, int a2, GFLNetSendDoneFunc callback);
static BOOL func_ov014_0217719c(GFLNetRecvFunc callback);
static BOOL func_ov014_021771a8(void);
static BOOL func_ov014_021771dc(void);
static BOOL func_ov014_021771e4(void);
static BOOL func_ov014_021771f8(void);
static int func_ov014_0217720c(void);
static int func_ov014_02177214(void);
static int func_ov014_0217721c(void);
static int func_ov014_0217723c(void);
static void func_ov014_0217725c(int a0);
static void func_ov014_02177264(int a0);
static BOOL func_ov014_0217726c(void);
static void func_ov014_02177274(int a0);

// pokebw2's sDevWireless
static NetDevWireless *data_ov014_021797a0;

// pokebw2's sNetDevWirelessTable
static GFLNetDevTable data_ov014_021796a0 = {
    .init = func_ov014_02177028,
    .unk08 = func_ov014_02177080,
    .update = func_ov014_02177094,
    .unk10 = func_ov014_021770a0,
    .unk14 = func_ov014_021770cc,
    .unk18 = func_ov014_021770ec,
    .unk1C = func_ov014_021770f8,
    .unk20 = func_ov014_021770fc,
    .unk24 = func_ov014_02177100,
    .unk28 = func_ov014_02177104,
    .unk2C = func_ov014_02177114,
    .unk30 = func_ov014_02177124,
    .unk34 = func_ov014_02177128,
    .unk38 = func_ov014_0217712c,
    .setDisconnectCallback = func_ov014_02177134,
    .unk40 = func_ov014_02177140,
    .unk44 = func_ov014_02177148,
    .unk48 = func_ov014_02177150,
    .unk4C = func_ov014_0217715c,
    .unk54 = func_ov014_02177160,
    .unk58 = func_ov014_02177168,
    .unk5C = func_ov014_0217717c,
    .getRecvData = func_ov014_02177184,
    .send = func_ov014_02177190,
    .setRecvCallback = func_ov014_0217719c,
    .unk6C = func_ov014_021771a8,
    .isConnected = func_ov014_021771dc,
    .unk74 = func_ov014_021771e4,
    .unk78 = func_ov014_021771f8,
    .getConnectBits = func_ov014_0217720c,
    .getNetId = func_ov014_02177214,
    .getSignalLevel = func_ov014_0217721c,
    .isError = func_ov014_0217723c,
    .unk8C = func_ov014_0217725c,
    .unkB8 = func_ov014_02177264,
    .unkBC = func_ov014_0217726c,
    .unkC0 = func_ov014_02177274,
};

// pokebw2's NetDevWireless_GetTable
const GFLNetDevTable *func_ov014_02177020(void) {
    return &data_ov014_021796a0;
}

// pokebw2's NetDevWireless_Init
static BOOL func_ov014_02177028(HeapID heapId, void *sys, WHCallback callback, void *work) {
    BOOL flag;

    if (data_ov014_021797a0 != NULL) {
        return FALSE;
    }
    data_ov014_021797a0 = GFL_HeapAllocate(heapId, sizeof(NetDevWireless), TRUE, "net_devwireless.c", 180);
    data_ov014_021797a0->sys = sys;
    flag = TRUE;
    if (func_0203d798()->bNetType != 5) {
        flag = FALSE;
    }
    return func_ov014_02177280(heapId, callback, work, flag);
}

// pokebw2's NetDevWireless_Begin
static BOOL func_ov014_02177080(void (*callback)(BOOL ok)) {
    if (callback != NULL) {
        callback(func_ov014_02177aa8());
    }
    return func_ov014_02177aa8();
}

// pokebw2's NetDevWireless_Update
static int func_ov014_02177094(u16 connectBits) {
    func_ov014_0217796c(connectBits);
    return TRUE;
}

// pokebw2's NetDevWireless_Unk10
static BOOL func_ov014_021770a0(int a0, int a1) {
    return func_ov014_021776c4();
}

// pokebw2's NetDevWireless_Finish
static BOOL func_ov014_021770a8(BOOL success) {
    if (data_ov014_021797a0->endCallback != NULL) {
        data_ov014_021797a0->endCallback(success);
    }
    GFL_HeapFree(data_ov014_021797a0);
    data_ov014_021797a0 = NULL;
    return TRUE;
}

// pokebw2's NetDevWireless_End
static BOOL func_ov014_021770cc(void (*endCallback)(BOOL ok)) {
    if (data_ov014_021797a0 != NULL) {
        data_ov014_021797a0->endCallback = endCallback;
        return func_ov014_02177eb0(func_ov014_021770a8);
    }
    return TRUE;
}

// pokebw2's NetDevWireless_ResetPipe
static BOOL func_ov014_021770ec(void) {
    func_ov014_02177560();
    return TRUE;
}

// pokebw2's NetDevWireless_ReturnTrue1C
static BOOL func_ov014_021770f8(void) {
    return TRUE;
}

// pokebw2's NetDevWireless_ReturnTrue20
static BOOL func_ov014_021770fc(void) {
    return TRUE;
}

// pokebw2's NetDevWireless_ReturnTrue24
static BOOL func_ov014_02177100(void) {
    return TRUE;
}

// pokebw2's NetDevWireless_GetBeaconData
static void *func_ov014_02177104(int index) {
    if (index >= 16) {
        return NULL;
    }
    return func_ov014_02177b2c(index);
}

// pokebw2's NetDevWireless_GetBeaconMac
static u8 *func_ov014_02177114(int index) {
    if (index >= 16) {
        return NULL;
    }
    return func_ov014_02177b58(index);
}

// pokebw2's NetDevWireless_ReturnTrue30
static BOOL func_ov014_02177124(void) {
    return TRUE;
}

// pokebw2's NetDevWireless_ReturnTrue34
static BOOL func_ov014_02177128(void) {
    return TRUE;
}

// pokebw2's NetDevWireless_Unk38
static BOOL func_ov014_0217712c(int a0) {
    return func_ov014_02177c08();
}

// pokebw2's NetDevWireless_SetDisconnectCallback
static BOOL func_ov014_02177134(void (*callback)(int netId)) {
    func_ov014_02179628(callback);
    return TRUE;
}

// pokebw2's NetDevWireless_Unk40
static int func_ov014_02177140(int a0, BOOL (*callback)(void)) {
    return func_ov014_02177d0c(a0);
}

// pokebw2's NetDevWireless_Unk44
static BOOL func_ov014_02177148(int a0, int a1) {
    return func_ov014_021775bc(a0);
}

// pokebw2's NetDevWireless_Unk48
static int func_ov014_02177150(BOOL a0, const u8 *mac, int index, int a3, void (*callback)(void)) {
    return func_ov014_02177e20(a0, mac, index, callback);
}

// pokebw2's NetDevWireless_Unk4C
static BOOL func_ov014_0217715c(const char *key, int numEntry, BOOL a2, int a3) {
    return FALSE;
}

// pokebw2's NetDevWireless_Unk54
static BOOL func_ov014_02177160(BOOL a0, int a1) {
    return func_ov014_021776c4();
}

// pokebw2's NetDevWireless_IsDataSharing
static BOOL func_ov014_02177168(void) {
    return func_ov014_02178e4c() == WH_SYSSTATE_DATASHARING;
}

// pokebw2's NetDevWireless_Unk5C
static BOOL func_ov014_0217717c(u8 *data) {
    return func_ov014_021793b8(data);
}

// pokebw2's NetDevWireless_GetRecvData
static u8 *func_ov014_02177184(int netId) {
    return func_ov014_02179394(netId);
}

// pokebw2's NetDevWireless_Send
static BOOL func_ov014_02177190(u8 *data, int size, int a2, GFLNetSendDoneFunc callback) {
    return func_ov014_02177c30(data, size, callback);
}

// pokebw2's NetDevWireless_SetRecvCallback
static BOOL func_ov014_0217719c(GFLNetRecvFunc callback) {
    func_ov014_02177310(callback);
    return TRUE;
}

// pokebw2's NetDevWireless_IsRunning
static BOOL func_ov014_021771a8(void) {
    switch (func_ov014_02178e4c()) {
    case WH_SYSSTATE_IDLE:
    case WH_SYSSTATE_SCANNING:
    case WH_SYSSTATE_BUSY:
    case WH_SYSSTATE_CONNECTED:
    case WH_SYSSTATE_DATASHARING:
    case WH_SYSSTATE_KEYSHARING:
    case WH_SYSSTATE_MEASURECHANNEL:
    case WH_SYSSTATE_CONNECT_FAIL:
    case WH_SYSSTATE_ERROR:
    case WH_SYSSTATE_FATAL:
        return TRUE;
    }
    return FALSE;
}

// pokebw2's NetDevWireless_IsConnected
static BOOL func_ov014_021771dc(void) {
    return func_ov014_021779f4();
}

// pokebw2's NetDevWireless_Unk74
static BOOL func_ov014_021771e4(void) {
    return !func_ov014_02177a94();
}

// pokebw2's NetDevWireless_IsIdle
static BOOL func_ov014_021771f8(void) {
    return func_ov014_02178e4c() == WH_SYSSTATE_IDLE;
}

// pokebw2's NetDevWireless_GetConnectBits
static int func_ov014_0217720c(void) {
    return func_ov014_02177c3c();
}

// pokebw2's NetDevWireless_GetNetId
static int func_ov014_02177214(void) {
    return func_ov014_02177c44();
}

// pokebw2's NetDevWireless_GetSignalLevel
static int func_ov014_0217721c(void) {
    if (func_0203d5d4() == 3) {
        return 3 - func_02010848();
    }
    return 3 - WM_GetLinkLevel();
}

// pokebw2's NetDevWireless_GetError
static int func_ov014_0217723c(void) {
    int error = func_ov014_02178e68();

    if (error == 5) {
        error = 0;
    }
    if (func_ov014_02177aec() && error == 0) {
        error = 20;
    }
    return error;
}

// pokebw2's NetDevWireless_Unk8C
static void func_ov014_0217725c(int a0) {
    func_ov014_02177b0c(a0);
}

// pokebw2's NetDevWireless_UnkB8
static void func_ov014_02177264(int a0) {
    func_ov014_02177edc(a0);
}

// pokebw2's NetDevWireless_UnkBC
static BOOL func_ov014_0217726c(void) {
    return func_ov014_02178034();
}

// pokebw2's NetDevWireless_UnkC0
static void func_ov014_02177274(int a0) {
    func_ov014_021780e4(a0);
}

