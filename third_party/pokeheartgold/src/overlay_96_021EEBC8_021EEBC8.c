#include "global.h"

#include "error_handling.h"

extern s32 ov96_0221B1BC;

u16 ov96_021EEBC8(u32 arg0);

u16 ov96_021EEBC8(u32 arg0) {
    if (arg0 >= 6U) {
        GF_AssertFail();
    }
    return (u16) * (s32 *)(&ov96_0221B1BC + arg0);
}

struct NetHandle;

typedef struct NetHandle NetHandle;

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
    NetNegotiation *negotiations[9];
    struct {
        u8 netId;
        u8 pending;
    } replies[8];
    u8 unk3C[2];

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

void ov96_021EEC0C(int netId, void *size, const void *data, void *work, NetHandle *handle, int mode);

void ov96_021EEBE4(int netId, void *size, const void *data, void *work, NetHandle *handle);

void ov96_021EEBE4(int netId, void *size, const void *data, void *work, NetHandle *handle) {
    ov96_021EEC0C(netId, size, data, work, handle, 1);
}
