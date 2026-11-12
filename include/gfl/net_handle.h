// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/net_handle.h, the lines in use so far.
#ifndef BWPORT_GFL_NET_HANDLE_H
#define BWPORT_GFL_NET_HANDLE_H

#include "types.h"
#include "struct_decls.h"

// A handle for each machine and one for the server: the exchange of MAC addresses and IDs when a machine connects,
// the timing numbers that synchronize the machines, and data a machine shares

#define GFL_NET_HANDLE_MAX 9
#define GFL_NET_NETID_SERVER 0xff

// What a machine sends to the server when it connects
typedef struct {
    u8 serverMac[6];
    u8 mac[6];
    u8 netId;
    u8 valid;
    u8 unkE;
    u8 unkF;
} NetNegotiation;

struct NetHandle {
    void *data;
    u32 dataSize;
    NetNegotiation *negotiations[GFL_NET_HANDLE_MAX];
    struct {
        u8 netId;
        u8 pending;
    } replies[8];
    u8 unk3C[2];
    // The machines whose negotiations have been sent
    u16 negotiationsSent;
    u16 serverNegotiationsSent;
    u8 unk42[2];
    u16 timingRecv;
    u16 timingSend;
    u8 unk48;
    u8 timingSendPending;
    u8 state;
    u8 unk4B[3];
    u8 infoPending;
    u8 unk4F;
};

NetHandle *func_0203996c(int netId);
// This machine's handle
NetHandle *func_020399a8(void);
// The machines whose negotiations have been accepted
int func_020399ec(void);
BOOL func_02039acc(void);
// Sends a timing number to synchronize on, which func_02039cb4 checks for (pokebw2's func_02040624 and func_02040664)
void func_02039c68(NetHandle *handle, u32 a1, u32 a2);
BOOL func_02039cb4(NetHandle *handle, u32 a1, u32 a2);

enum {
    NET_HANDLE_STATE_NONE,
    NET_HANDLE_STATE_1,
    NET_HANDLE_STATE_SEND_NEGOTIATION,
    // The machine's negotiation has been accepted
    NET_HANDLE_STATE_NEGOTIATED = 5,
    NET_HANDLE_STATE_REJECTED = 7,
};

void func_02039a2c(int netId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Forgets a machine, as when it disconnects
void func_0203a338(int netID);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Forgets a machine, as when it disconnects

BOOL func_020399b8(NetHandle *handle);
u8 func_020399dc(NetHandle *handle);
// Command handlers: a machine's timing number, negotiation, and shared data
void func_02039b5c(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_02039c2c(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_02039c3c(NetHandle *handle, u16 timing);
BOOL func_02039ca0(NetHandle *handle, u16 timing);
void func_0203a150(int netId, int size, NetNegotiation *negotiation, void *work, NetHandle *handle);
void func_0203a27c(int netId, int size, u8 *data);
void func_0203a424(NetHandle *handle, int a1, int a2, int a3);
void func_0203a458(NetHandle *handle, int value);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// The machine with a handle, or GFL_NET_NETID_SERVER
int func_020395ec(NetHandle *handle);
// Create and free the handles; the argument is not used
void func_02039834(void *sys);
void func_020398b4(void *sys);
void func_02039d94(void);
void func_0203a020(int netId, int size, const void *data, void *work, NetHandle *handle);
// Forgets a machine, as when it disconnects
void func_0203a388(int netID, int size, const void *data, void *work, NetHandle *handle);



#endif // BWPORT_GFL_NET_HANDLE_H
