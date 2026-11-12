// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/delivery_irc.c, ported by tools/port_file.py.
#include "types.h"
#include "field/delivery_irc.h"
#include "field/net_devirc.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/random.h"
#include "gfl/std.h"

#define DELIVERY_IRC_BUFFER_SIZE 0x5e8

// What the sender sends before the data
typedef struct {
    u32 seed;
    u16 crc;
} DeliveryIrcHeader;

// What the receiver wants
typedef struct {
    u32 region;
    u32 mask;
} DeliveryIrcWanted;

typedef struct DeliveryIrcWork DeliveryIrcWork;
typedef void (*DeliveryIrcSeq)(void *work);

struct DeliveryIrcWork {
    DeliveryIrcInit aInit;
    u8 buffer[DELIVERY_IRC_BUFFER_SIZE];
    // The data being sent
    DeliveryData *sendData;
    u32 seed;
    // 1 when the sender has no data for the receiver
    u32 noData;
    u16 negotiated;
    u16 crc;
    u8 result;
    u8 isSender;
    u8 found;
    u8 index;
    DeliveryIrcSeq seq;
    DeliveryIrcWanted wanted;
};

static void func_ov010_02157948(void *work, int netId);
static void func_ov010_02157954(DeliveryIrcWork *pWork, DeliveryIrcSeq seq);
static void func_ov010_02157960(DeliveryIrcWork *pWork, DeliveryIrcSeq seq, u32 line);
static void func_ov010_02157968(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov010_0215796c(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov010_02157980(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov010_02157998(int netId, int size, const void *data, void *work, NetHandle *handle);
static void *func_ov010_021579a4(int netId, void *work, int size);
static void func_ov010_021579ac(void *work);
static void func_ov010_021579b0(void *work);
static void func_ov010_02157a30(void *work);
static void func_ov010_02157a58(void *work);
static void func_ov010_02157a78(void *work);
static void func_ov010_02157ac8(void *work);
static void func_ov010_02157af0(void *work);
static void func_ov010_02157b88(void *work);
static void func_ov010_02157c48(void *work);
static void func_ov010_02157d0c(void *work);
static void func_ov010_02157d68(void *work);
static void func_ov010_02157d98(void *work);
static void func_ov010_02157db8(void *work);
static BOOL func_ov010_02157e08(DeliveryIrcWork *pWork);

static const NetCommand data_ov010_02170048[] = {
    {func_ov010_02157968, func_ov010_021579a4},
    {func_ov010_0215796c, NULL},
    {func_ov010_02157980, NULL},
    {func_ov010_02157998, NULL},
};

static GFLNetInitData data_ov010_02172808 = {
    data_ov010_02170048,
    NELEMS(data_ov010_02170048),
    NULL,
    func_ov010_02157948,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    {0},
    NULL,
    NULL,
    0,
    {1, 0, 0, 0, 0x80, 0x13, 0, 0},
    HEAPID_USER,
    0xd,
    0xf,
    0x10,
    0xf0,
    0,
    2,
    0x40,
    0x10,
    1,
    0,
    3,
    1,
    0x1f,
    {0xfe, 0xff, 0x0, 0x0},
    0,
    0,
};

static void func_ov010_02157948(void *work, int netId) {
    DeliveryIrcWork *pWork = work;

    pWork->negotiated = TRUE;
}

static void func_ov010_02157954(DeliveryIrcWork *pWork, DeliveryIrcSeq seq) {
    pWork->seq = seq;
}

static void func_ov010_02157960(DeliveryIrcWork *pWork, DeliveryIrcSeq seq, u32 line) {
    func_ov010_02157954(pWork, seq);
}

static void func_ov010_02157968(int netId, int size, const void *data, void *work, NetHandle *handle) {
}

static void func_ov010_0215796c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    DeliveryIrcWork *pWork = work;
    const DeliveryIrcHeader *header = data;

    pWork->crc = header->crc;
    pWork->seed = header->seed;
}

static void func_ov010_02157980(int netId, int size, const void *data, void *work, NetHandle *handle) {
    DeliveryIrcWork *pWork = work;
    const DeliveryIrcWanted *wanted = data;

    pWork->wanted = *wanted;
}

static void func_ov010_02157998(int netId, int size, const void *data, void *work, NetHandle *handle) {
    DeliveryIrcWork *pWork = work;
    const u32 *noData = data;

    pWork->noData = *noData;
}

static void *func_ov010_021579a4(int netId, void *work, int size) {
    DeliveryIrcWork *pWork = work;

    return pWork->buffer;
}

static void func_ov010_021579ac(void *work) {
}

// The receiver checks the data that came
static void func_ov010_021579b0(void *work) {
    DeliveryIrcWork *pWork = work;

    if (func_0203cd98()) {
        return;
    }
    if (pWork->isSender == FALSE) {
        if (pWork->noData == TRUE) {
            pWork->result = 3;
        } else {
            pWork->result = 1;
            MI_CpuCopy8(pWork->buffer, pWork->aInit.data[0].pData, pWork->aInit.data[0].datasize);
            if (getCRC16(pWork->aInit.data[0].pData, pWork->aInit.data[0].datasize) != pWork->crc) {
                pWork->result = 2;
            }
            decryptData(pWork->aInit.data[0].pData, pWork->aInit.data[0].datasize, pWork->seed);
        }
    } else if (pWork->found == TRUE) {
        pWork->result = 1;
    } else {
        pWork->result = 3;
    }
    func_ov010_02157960(pWork, func_ov010_021579ac, 248);
}

static void func_ov010_02157a30(void *work) {
    if (func_ov015_02179d3c()) {
        func_0203ceac(NULL);
    }
    func_ov010_02157960(work, func_ov010_021579b0, 258);
}

static void func_ov010_02157a58(void *work) {
    func_0203ceac(NULL);
    func_ov010_02157960(work, func_ov010_021579b0, 266);
}

static void func_ov010_02157a78(void *work) {
    DeliveryIrcWork *pWork = work;

    if (func_02039cb4(func_020399a8(), 35, (u8)pWork->aInit.code)) {
        if (func_0203d378()) {
            func_ov015_0217998c(300);
            func_ov010_02157960(pWork, func_ov010_02157a30, 275);
        } else {
            func_ov010_02157960(pWork, func_ov010_02157a58, 278);
        }
    }
}

static void func_ov010_02157ac8(void *work) {
    DeliveryIrcWork *pWork = work;

    func_02039c68(func_020399a8(), 35, (u8)pWork->aInit.code);
    func_ov010_02157960(pWork, func_ov010_02157a78, 287);
}

// The sender sends the data
static void func_ov010_02157af0(void *work) {
    DeliveryIrcWork *pWork = work;
    DeliveryData *data;

    if (func_02039cb4(func_020399a8(), 24, (u8)pWork->aInit.code)) {
        if (pWork->isSender) {
            if (pWork->found == TRUE) {
                data = &pWork->aInit.data[pWork->index];
                if (func_0203d3f8(func_020399a8(), 0xff, pWork->aInit.code << 8, data->datasize, data->pData, 0, FALSE,
                                  TRUE)) {
                    func_ov010_02157960(pWork, func_ov010_02157ac8, 302);
                }
            } else {
                func_ov010_02157960(pWork, func_ov010_02157ac8, 307);
            }
        } else {
            func_ov010_02157960(pWork, func_ov010_02157ac8, 311);
        }
    }
}

// The sender sends the header, or that it has no data
static void func_ov010_02157b88(void *work) {
    DeliveryIrcWork *pWork = work;
    DeliveryIrcHeader header;

    if (pWork->found == TRUE) {
        header.crc = getCRC16(pWork->sendData->pData, pWork->sendData->datasize);
        header.seed = pWork->seed;
        if (func_0203d3a8(func_020399a8(), (u16)((pWork->aInit.code << 8) + 1), sizeof(DeliveryIrcHeader), &header)) {
            func_02039c68(func_020399a8(), 24, (u8)pWork->aInit.code);
            pWork->negotiated = FALSE;
            func_ov010_02157960(pWork, func_ov010_02157af0, 338);
        }
    } else {
        pWork->noData = TRUE;
        if (func_0203d3a8(func_020399a8(), (u16)((pWork->aInit.code << 8) + 3), sizeof(pWork->noData),
                          &pWork->noData)) {
            func_02039c68(func_020399a8(), 24, (u8)pWork->aInit.code);
            pWork->negotiated = FALSE;
            func_ov010_02157960(pWork, func_ov010_02157af0, 354);
        }
    }
}

// The sender looks for data the receiver wants
static void func_ov010_02157c48(void *work) {
    DeliveryIrcWork *pWork = work;
    DeliveryData *data;
    u32 i;

    if (func_02039cb4(func_020399a8(), 23, (u8)pWork->aInit.code)) {
        if (pWork->isSender) {
            data = NULL;
            pWork->found = FALSE;
            for (i = 0; i < pWork->aInit.dataNum; i++) {
                data = &pWork->aInit.data[i];
                if (data->region == pWork->wanted.region && (data->mask & pWork->wanted.mask)) {
                    pWork->index = i;
                    pWork->found = TRUE;
                    break;
                }
            }
            func_ov010_02157960(pWork, func_ov010_02157b88, 392);
            if (pWork->found == TRUE) {
                pWork->seed = GFL_RandomLC(0);
                decryptData(data->pData, data->datasize, pWork->seed);
                pWork->sendData = data;
            }
        } else {
            func_02039c68(func_020399a8(), 24, (u8)pWork->aInit.code);
            func_ov010_02157960(pWork, func_ov010_02157af0, 403);
        }
    }
}

// The receiver says what it wants
static void func_ov010_02157d0c(void *work) {
    DeliveryIrcWork *pWork = work;

    if (pWork->isSender == FALSE) {
        pWork->wanted.region = pWork->aInit.data[0].region;
        pWork->wanted.mask = pWork->aInit.data[0].mask;
        if (func_0203d3a8(func_020399a8(), (u16)((pWork->aInit.code << 8) + 2), sizeof(DeliveryIrcWanted),
                          &pWork->wanted)) {
            func_ov010_02157960(pWork, func_ov010_02157c48, 418);
        }
    } else {
        func_ov010_02157960(pWork, func_ov010_02157c48, 422);
    }
}

static void func_ov010_02157d68(void *work) {
    DeliveryIrcWork *pWork = work;

    if (func_0203d1b4() > 1) {
        func_02039c68(func_020399a8(), 23, (u8)pWork->aInit.code);
        func_ov010_02157960(pWork, func_ov010_02157d0c, 431);
    }
}

static void func_ov010_02157d98(void *work) {
    func_0203d0dc(NULL);
    func_ov010_02157960(work, func_ov010_02157d68, 439);
}

static void func_ov010_02157db8(void *work) {
    if (func_0203cd98() == TRUE) {
        func_ov010_02157960(work, func_ov010_02157d98, 446);
    }
}

void *func_ov010_02157dd8(const DeliveryIrcInit *init) {
    DeliveryIrcWork *pWork = GFL_HeapAllocate(init->heapId, sizeof(DeliveryIrcWork), TRUE, "delivery_irc.c", 461);

    MI_CpuCopy8(init, &pWork->aInit, sizeof(DeliveryIrcInit));
    return pWork;
}

static BOOL func_ov010_02157e08(DeliveryIrcWork *pWork) {
    if (func_0203cd98()) {
        return FALSE;
    }
    data_ov010_02172808.gameCommandBase = pWork->aInit.code;
    func_0203cbac(&data_ov010_02172808, NULL, pWork);
    return TRUE;
}

BOOL func_ov010_02157e34(void *work) {
    DeliveryIrcWork *pWork = work;
    BOOL started = func_ov010_02157e08(pWork);

    pWork->isSender = FALSE;
    pWork->found = FALSE;
    pWork->index = 0;
    pWork->noData = 0;
    func_ov010_02157960(pWork, func_ov010_02157db8, 561);
    return started;
}

u8 func_ov010_02157e6c(void *work) {
    DeliveryIrcWork *pWork = work;

    return pWork->result;
}

void func_ov010_02157e78(void *work) {
    DeliveryIrcWork *pWork = work;

    if (pWork->seq != NULL) {
        pWork->seq(pWork);
    }
}

void func_ov010_02157e8c(void *work) {
    GFL_HeapFree(work);
    func_0203ceac(NULL);
}
