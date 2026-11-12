// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov031/net_devirc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// Overlay 31, named descriptively (no string names the file; its asserts name the infrared system NetIrcSys, and its
// table is a GFLNetDevTable like net_devwireless.c's): the GFL net device for infrared. It is the table of wrappers
// over NetIrcSys (net_irc.c), which the other files of the overlay carry down to the card's infrared chip. Overlay 31
// is four original files, probably in this order: this one, net_irc.c, irc_link.c and irc_spi.c; MWCC sorts a file's
// .bss by size, and the original's .bss order is only possible split so.
#include "field/net_devirc.h"
#include "field/net_irc.h"
#include "field/irc_link.h"

static BOOL func_ov015_021797c0(HeapID heapId, void *sys, BOOL (*callback)(BOOL success), void *work);
static BOOL func_ov015_021797e0(void (*callback)(BOOL ok));
static int func_ov015_021797ec(u16 connectBits);
static BOOL func_ov015_021797f8(int a0, int a1);
static BOOL func_ov015_021797fc(void (*callback)(BOOL ok));
static BOOL func_ov015_02179808(void);
static BOOL func_ov015_02179810(void);
static BOOL func_ov015_02179824(u8 *data, int size, int a2, GFLNetSendDoneFunc callback);
static BOOL func_ov015_02179840(GFLNetRecvFunc callback);
static int func_ov015_0217984c(void);
static int func_ov015_02179860(void);
static void func_ov015_02179868(void);
static BOOL func_ov015_02179870(void);
static BOOL func_ov015_02179878(void);
static BOOL func_ov015_02179880(void);
static BOOL func_ov015_02179888(void);
static BOOL func_ov015_02179890(void);
static BOOL func_ov015_02179898(void);
static int func_ov015_021798a0(void);
static BOOL func_ov015_021798b4(void);
static void func_ov015_021798bc(void);
static void func_ov015_021798c4(int frames);
static void func_ov015_021798cc(void *filter);

// pokebw2's NetDevIrc_Init
static BOOL func_ov015_021797c0(HeapID heapId, void *sys, BOOL (*callback)(BOOL success), void *work) {
    GFLNetInitData *init = func_0203d798();

    func_ov015_0217993c(func_0203d798()->unk68.word, init->gameCommandBase);
    func_ov015_021799cc();
    return TRUE;
}

// pokebw2's NetDevIrc_Begin
static BOOL func_ov015_021797e0(void (*callback)(BOOL ok)) {
    func_ov015_02179fc4();
    return TRUE;
}

// pokebw2's NetDevIrc_Update
static int func_ov015_021797ec(u16 connectBits) {
    func_ov015_02179aac();
    return 0;
}

// pokebw2's NetDevIrc_ReturnTrue10
static BOOL func_ov015_021797f8(int a0, int a1) {
    return TRUE;
}

// pokebw2's NetDevIrc_End
static BOOL func_ov015_021797fc(void (*callback)(BOOL ok)) {
    func_ov015_02179998();
    return TRUE;
}

// pokebw2's NetDevIrc_IsRunning
static BOOL func_ov015_02179808(void) {
    return func_ov015_021799f0();
}

// pokebw2's NetDevIrc_IsIdle
static BOOL func_ov015_02179810(void) {
    if (!func_ov015_021799f0()) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's NetDevIrc_Send
static BOOL func_ov015_02179824(u8 *data, int size, int a2, GFLNetSendDoneFunc callback) {
    func_ov015_02179a4c(data, size, 0);
    if (callback != NULL) {
        callback(TRUE);
    }
    return TRUE;
}

// pokebw2's NetDevIrc_SetRecvCallback
static BOOL func_ov015_02179840(GFLNetRecvFunc callback) {
    func_ov015_021799b4(callback);
    return TRUE;
}

// pokebw2's NetDevIrc_GetNetId
static int func_ov015_0217984c(void) {
    if (func_ov015_02179d3c()) {
        return func_ov015_02179b24();
    }
    return 0;
}

// pokebw2's NetDevIrc_IsError
static int func_ov015_02179860(void) {
    return func_ov015_02179cac();
}

// pokebw2's NetDevIrc_Poll
static void func_ov015_02179868(void) {
    func_ov015_02179d80();
}

// pokebw2's NetDevIrc_CanSend
static BOOL func_ov015_02179870(void) {
    return func_ov015_02179a18();
}

// pokebw2's NetDevIrc_GetSendTurn
static BOOL func_ov015_02179878(void) {
    return func_ov015_021799fc();
}

// pokebw2's NetDevIrc_GetPeerSeen
static BOOL func_ov015_02179880(void) {
    return func_ov015_02179d68();
}

// pokebw2's NetDevIrc_IsLinked
static BOOL func_ov015_02179888(void) {
    return func_ov015_02179d78();
}

// pokebw2's NetDevIrc_IsConnectedSlot
static BOOL func_ov015_02179890(void) {
    return func_ov015_02179d3c();
}

// pokebw2's NetDevIrc_IsInitialized
static BOOL func_ov015_02179898(void) {
    return func_ov015_021799f0();
}

// pokebw2's NetDevIrc_GetConnectBits
static int func_ov015_021798a0(void) {
    if (func_ov015_02179d3c()) {
        return 3;
    }
    return 0;
}

// pokebw2's NetDevIrc_GetUnkCF0
static BOOL func_ov015_021798b4(void) {
    return func_ov015_02179a08();
}

// pokebw2's NetDevIrc_StartLink
static void func_ov015_021798bc(void) {
    func_ov015_02179ce4();
}

// pokebw2's NetDevIrc_SetTimeoutSlot
static void func_ov015_021798c4(int frames) {
    func_ov015_0217998c(frames);
}

// pokebw2's NetDevIrc_SetPeerFilterSlot
static void func_ov015_021798cc(void *filter) {
    func_ov015_021799c0(filter);
}

// pokebw2's NetDevIrcTable
static GFLNetDevTable data_ov015_0217a9e0 = {
    .init = func_ov015_021797c0,
    .unk08 = func_ov015_021797e0,
    .update = func_ov015_021797ec,
    .unk10 = func_ov015_021797f8,
    .unk14 = func_ov015_021797fc,
    .send = func_ov015_02179824,
    .setRecvCallback = func_ov015_02179840,
    .unk6C = func_ov015_02179808,
    .isConnected = func_ov015_02179890,
    .unk74 = func_ov015_02179810,
    .unk78 = func_ov015_02179898,
    .getConnectBits = func_ov015_021798a0,
    .getNetId = func_ov015_0217984c,
    .isError = func_ov015_02179860,
    .unk9C = func_ov015_02179868,
    .unkA0 = func_ov015_02179870,
    .unkA4 = func_ov015_02179878,
    .unkA8 = func_ov015_02179880,
    .unkAC = func_ov015_02179888,
    .unkB0 = func_ov015_021798b4,
    .unkB4 = func_ov015_021798bc,
    .unkC4 = func_ov015_021798c4,
    .unkC8 = func_ov015_021798cc,
};

// pokebw2's NetDevIrc_GetTable
const GFLNetDevTable *func_ov015_021798d4(void) {
    return &data_ov015_0217a9e0;
}
