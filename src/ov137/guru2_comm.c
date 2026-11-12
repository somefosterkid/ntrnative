// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov216/guru2_comm.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/guru2.h"
#include "field/union_app.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/net_system.h"
#include "gfl/std.h"
#include "nitro/math.h"

// guru2_comm.c, which the ROM names: the Spin Trade's net commands. Names are ours

static UnionApp *func_ov137_021dcafc(Guru2CommWork *g2c);
static BOOL func_ov137_021dcb94(Guru2CommWork *g2c, u32 command, const void *data, u32 size);
static void func_ov137_021dcbcc(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov137_021dcbfc(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov137_021dcc24(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov137_021dccf0(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov137_021dcd0c(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov137_021dcd14(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov137_021dcd34(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov137_021dcd44(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov137_021dcd5c(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov137_021dcd6c(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov137_021dcd7c(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov137_021dcd8c(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov137_021dcda0(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov137_021dcdb0(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov137_021dcdc0(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov137_021dcdd8(int netId, int size, const void *data, void *work, NetHandle *handle);
static void *func_ov137_021dcdf0(int netId, void *work, int size);

// The commands from GURU2_NET_CMD_BASE
// pokebw2's sGuru2Commands
static const NetCommand data_ov137_021e1a68[GURU2_NET_CMD_COUNT] = {
    { func_ov137_021dcd14, NULL },
    { func_ov137_021dcc24, NULL },
    { func_ov137_021dccf0, NULL },
    { func_ov137_021dcd0c, NULL },
    { func_ov137_021dcbfc, NULL },
    { func_ov137_021dcd34, NULL },
    { func_ov137_021dcd44, NULL },
    { func_ov137_021dcd7c, NULL },
    { func_ov137_021dcd8c, NULL },
    { func_ov137_021dcd5c, NULL },
    { func_ov137_021dcd6c, NULL },
    { func_ov137_021dcda0, NULL },
    { func_ov137_021dcdb0, NULL },
    { func_ov137_021dcdc0, NULL },
    { func_ov137_021dcdd8, NULL },
    { func_ov137_021dcbcc, func_ov137_021dcdf0 },
};

// pokebw2's Guru2Comm_GetUnionApp
static UnionApp *func_ov137_021dcafc(Guru2CommWork *g2c) {
    return g2c->g2p->param->app;
}

// pokebw2's Guru2Comm_Create
Guru2CommWork *func_ov137_021dcb04(Guru2ProcWork *g2p, u32 heapId) {
    Guru2CommWork *g2c;

    g2c = GFL_HeapAllocate(heapId, sizeof(Guru2CommWork), FALSE, "guru2_comm.c", 0x98);
    MI_CpuFill8(g2c, 0, sizeof(Guru2CommWork));
    g2c->g2p = g2p;
    g2c->blockRecvBuffer =
        GFL_HeapAllocate(heapId, GURU2_COMM_BLOCK_SIZE * GURU2_MEMBER_MAX, FALSE, "guru2_comm.c", 0x9f);
    g2c->blocks = GFL_HeapAllocate(heapId, GURU2_COMM_BLOCK_SIZE * GURU2_MEMBER_MAX, FALSE, "guru2_comm.c", 0xa1);
    return g2c;
}

// pokebw2's Guru2Comm_Free
void func_ov137_021dcb60(Guru2CommWork *g2c) {
    GFL_HeapFree(g2c->blockRecvBuffer);
    GFL_HeapFree(g2c->blocks);
    GFL_HeapFree(g2c);
}

// pokebw2's Guru2Comm_AddCommands
void func_ov137_021dcb78(Guru2CommWork *g2c) {
    func_0203a574(GURU2_NET_CMD_BASE, data_ov137_021e1a68, GURU2_NET_CMD_COUNT, g2c);
}

// pokebw2's Guru2Comm_End
void func_ov137_021dcb90(Guru2CommWork *g2c) {
}

// pokebw2's Guru2Comm_SendData
static BOOL func_ov137_021dcb94(Guru2CommWork *g2c, u32 command, const void *data, u32 size) {
    NetHandle *handle = func_020399a8();
    Guru2CommSendBuffer *send = &g2c->send;

    MI_CpuCopy8(data, send->data, size);
    return func_0203d3a8(handle, (u16)command, size + 4, send->data);
}

// pokebw2's Guru2Comm_Send
BOOL func_ov137_021dcbc4(Guru2CommWork *g2c, u32 command, const void *data, u32 size) {
    return func_ov137_021dcb94(g2c, command, data, size);
}

// pokebw2's Guru2Comm_RecvBlock
static void func_ov137_021dcbcc(int netId, int size, const void *data, void *work, NetHandle *handle) {
    Guru2CommWork *g2c = work;

    g2c->blockBits |= 1 << netId;
    MI_CpuCopy8(data, func_ov137_021dce00(g2c, netId), GURU2_COMM_BLOCK_SIZE);
}

// pokebw2's Guru2Comm_RecvStart
static void func_ov137_021dcbfc(int netId, int size, const void *data, void *work, NetHandle *handle) {
    Guru2CommWork *g2c = work;

    if (func_02039338() != 0) {
        g2c->startCount = 0;
        g2c->started = TRUE;
        func_ov137_021de08c(g2c->g2p->g2r, GURU2_RECEIPT_SEQ_START, netId);
    }
}

// pokebw2's Guru2Comm_RecvSignal
static void func_ov137_021dcc24(int netId, int size, const void *data, void *work, NetHandle *handle) {
    const Guru2CommSignal *signal = data;
    Guru2CommWork *g2c = work;

    if (netId != 0) {
        // A member's signal reaches the parent, which answers it
        if (func_02039338() == 0) {
            Guru2CommSignal answer = *signal;

            answer.netId = netId;
            answer.count = g2c->joinCount;
            switch (signal->type) {
            case GURU2_SIGNAL_LEAVE_REQUEST:
                if (g2c->joinCount != func_ov022_021841a4(func_ov137_021dcafc(g2c))
                    || g2c->joinCount != MATH_CountPopulation(func_ov022_021841a0(func_ov137_021dcafc(g2c)))) {
                    answer.ok = FALSE;
                } else {
                    g2c->leavingBits |= 1 << netId;
                    answer.ok = TRUE;
                }
                break;
            case GURU2_SIGNAL_LEAVE:
                break;
            }
            func_ov137_021dcbc4(g2c, GURU2_NET_CMD_SIGNAL, &answer, sizeof(answer));
        }
    } else {
        switch (signal->type) {
        case GURU2_SIGNAL_LEAVE_REQUEST: {
            u8 id = signal->netId;

            if (id == func_02039338()) {
                if (!signal->ok) {
                    func_ov137_021de08c(g2c->g2p->g2r, GURU2_RECEIPT_SEQ_CANCEL_REFUSED, id);
                } else {
                    g2c->signalCount = signal->count;
                    func_ov137_021de08c(g2c->g2p->g2r, GURU2_RECEIPT_SEQ_CANCEL_OK, id);
                }
            }
            break;
        }
        case GURU2_SIGNAL_LEAVE:
            func_ov137_021de08c(g2c->g2p->g2r, GURU2_RECEIPT_SEQ_MEMBER_LEFT, signal->netId);
            break;
        }
    }
}

// pokebw2's Guru2Comm_RecvCancel
static void func_ov137_021dccf0(int netId, int size, const void *data, void *work, NetHandle *handle) {
    Guru2CommWork *g2c = work;

    if (func_02039338() != 0) {
        func_ov137_021de08c(g2c->g2p->g2r, GURU2_RECEIPT_SEQ_PARENT_CANCEL_MSG, 0);
    }
}

// pokebw2's Guru2Comm_Recv3
static void func_ov137_021dcd0c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    func_02039338();
}

// pokebw2's Guru2Comm_RecvJoin
static void func_ov137_021dcd14(int netId, int size, const void *data, void *work, NetHandle *handle) {
    Guru2CommWork *g2c = work;
    const u8 *id = data;

    func_ov137_021de17c(g2c->g2p->g2r, GURU2_RECEIPT_SEQ_1, *id);
    if (func_02039338() == 0) {
        g2c->unk34 = 0;
    }
}

// pokebw2's Guru2Comm_Recv5
static void func_ov137_021dcd34(int netId, int size, const void *data, void *work, NetHandle *handle) {
    Guru2CommWork *g2c = work;
    const u16 *bits = data;

    g2c->unk40 |= *bits;
}

// pokebw2's Guru2Comm_Recv6
static void func_ov137_021dcd44(int netId, int size, const void *data, void *work, NetHandle *handle) {
    Guru2CommWork *g2c = work;

    g2c->unk42 |= 1 << netId;
}

// pokebw2's Guru2Comm_Recv9
static void func_ov137_021dcd5c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    Guru2CommWork *g2c = work;

    func_ov137_021e172c(g2c->g2p->g2m, data);
}

// pokebw2's Guru2Comm_Recv10
static void func_ov137_021dcd6c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    Guru2CommWork *g2c = work;
    const u32 *value = data;

    func_ov137_021e1758(g2c->g2p->g2m, *value);
}

// pokebw2's Guru2Comm_Recv7
static void func_ov137_021dcd7c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    Guru2CommWork *g2c = work;
    const u8 *value = data;

    func_ov137_021e168c(g2c->g2p->g2m, *value);
}

// pokebw2's Guru2Comm_Recv8
static void func_ov137_021dcd8c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    Guru2CommWork *g2c = work;

    if (netId == 0) {
        func_ov137_021e1694(g2c->g2p->g2m, data);
    }
}

// pokebw2's Guru2Comm_Recv11
static void func_ov137_021dcda0(int netId, int size, const void *data, void *work, NetHandle *handle) {
    Guru2CommWork *g2c = work;
    const u32 *value = data;

    func_ov137_021e180c(g2c->g2p->g2m, netId, *value);
}

// pokebw2's Guru2Comm_Recv12
static void func_ov137_021dcdb0(int netId, int size, const void *data, void *work, NetHandle *handle) {
    Guru2CommWork *g2c = work;

    func_ov137_021e1814(g2c->g2p->g2m, data);
}

// pokebw2's Guru2Comm_Recv13
static void func_ov137_021dcdc0(int netId, int size, const void *data, void *work, NetHandle *handle) {
    Guru2CommWork *g2c = work;

    g2c->unk48 |= 1 << netId;
}

// pokebw2's Guru2Comm_Recv14
static void func_ov137_021dcdd8(int netId, int size, const void *data, void *work, NetHandle *handle) {
    Guru2CommWork *g2c = work;

    g2c->unk4A |= 1 << netId;
}

// pokebw2's Guru2Comm_GetBlockRecvBuffer
static void *func_ov137_021dcdf0(int netId, void *work, int size) {
    Guru2CommWork *g2c = work;

    return g2c->blockRecvBuffer + netId * GURU2_COMM_BLOCK_SIZE;
}

// pokebw2's Guru2Comm_GetBlock
void *func_ov137_021dce00(Guru2CommWork *g2c, int netId) {
    return g2c->blocks + netId * GURU2_COMM_BLOCK_SIZE;
}
