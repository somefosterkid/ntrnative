#include "global.h"
struct UnkStruct_ov22_02258EE4;
#include "gf_gfx_loader.h"
#include "pokepic.h"
#include "overlay_41_unk_struct_ov22_02258ee4.h"
#include "overlay_41_unk_struct_ov22_02259560.h"

typedef struct UnkStruct_ov41_022495A4_v0 {
    u8 unk_0[0x4];
    void *unk_4;
    u8 unk_8[0x8];
    void *unk_10;
    u8 unk_14[0x10];
    s32 unk_24;
    s32 unk_28;
} UnkStruct_ov41_022495A4_v0;

typedef struct UnkStruct_ov41_02249574_v1 {
    s32 *unk_0;
    s32 unk_4;
} UnkStruct_ov41_02249574_v1;

s32 ov41_0224AC08(void *, s32, s32, s32);

void ov41_02249574(struct UnkStruct_ov22_02258EE4 *arg0);

s32 ov41_02248114(void *, s32, s32);
s32 ov41_02249AF4(UnkStruct_ov22_02259560 *param0, int param1, int param2);
void ov41_022495A4(void **arg0, s32 arg1, s32 arg2);

void *ov41_022463DC(void *, void *, s32);
void ov41_022495C8(void *arg0, void *arg1);

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
void ov41_02249604(void *netId, void *size, const void *data, void *work, NetHandle *handle, int mode);
void ov41_022495F0(void *netId, void *size, const void *data, void *work, NetHandle *handle);

void ov41_02249574(UnkStruct_ov22_02258EE4 *arg0) {
    s32 v0;
    UnkStruct_ov41_02249574_v1 *v1;

    v1 = *((void * *)&arg0->unk_10);
    v0 = v1->unk_4;
    if (v0 == 0) {
        ov41_0224AC08(*((void **)&arg0->unk_0C), 0x1B, 0xD8, *v1->unk_0);
        return;
    }
    switch (v0) {
    case 1:
        ov41_0224AC08(*((void **)&arg0->unk_0C), 0x1B, 0xDA, *v1->unk_0);
        return;
    }
}

void ov41_022495A4(void **arg0, s32 arg1, s32 arg2) {
    UnkStruct_ov41_022495A4_v0 *v0;

    v0 = *arg0;
    ov41_02249AF4(v0->unk_10, arg1, arg2);
    ov41_02248114(v0->unk_4, arg1 - v0->unk_24, arg2 - v0->unk_28);
    v0->unk_24 = arg1;
    v0->unk_28 = arg2;
}

void ov41_022495C8(void *arg0, void *arg1) {
    u8 *new_var;
    new_var = ((u8 *)ov41_022463DC(arg0, GfGfxLoader_LoadFromNarc((enum NarcId)(*((u16 *)arg1)), (s32)(*((u16 *)(((u8 *)arg1) + 2))), 0, HEAP_ID_14, 0), 0x76)) + 0x14;
    UnscanPokepic(*((u8 **)new_var), (enum NarcId)(*((u16 *)arg1)));
}

void ov41_022495F0(void *netId, void *size, const void *data, void *work, NetHandle *handle) {
    ov41_02249604(netId, size, data, work, handle, 0);
}
