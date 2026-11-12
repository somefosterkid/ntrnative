// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/delivery_beacon.c, ported by tools/port_file.py.
#include "types.h"
#include "field/delivery_beacon.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_whpipe.h"
#include "gfl/std.h"

#define DELIVERY_BEACON_ONCE_NUM 84

// A beacon of the data
typedef struct {
    u16 crc;
    u16 unk2;
    // The seed of the data's encryption
    u32 seed;
    u8 data[DELIVERY_BEACON_ONCE_NUM];
    // Which of the beacons this is, from 1, and how many there are
    u8 index;
    u8 count;
    u8 unk5E;
    u8 region;
    u32 mask;
} DeliveryBeaconData;

typedef struct DeliveryBeaconWork DeliveryBeaconWork;
typedef void (*DeliveryBeaconSeq)(void *work);

struct DeliveryBeaconWork {
    DeliveryInit aInit;
    DeliveryBeaconData beacons[7][DELIVERY_BEACON_MAX_NUM];
    int sendIndex;
    // The unk5E of the beacons taken, 0xff until the first one
    u8 unk5E;
    DeliveryBeaconSeq seq;
};

static void func_ov010_0215765c(void *work, int netId);
static void func_ov010_02157660(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov010_02157664(void *work, int netId);
static void func_ov010_02157668(void *work);
static void *func_ov010_0215766c(void *work);
static int func_ov010_021576a8(void *work);
static BOOL func_ov010_021576ac(u32 a, u32 b);
static void func_ov010_021576b8(DeliveryBeaconWork *pWork, DeliveryBeaconSeq seq);
static void func_ov010_021576c4(DeliveryBeaconWork *pWork, DeliveryBeaconSeq seq, u32 line);
static void func_ov010_021576cc(void *work);
static void func_ov010_02157700(DeliveryBeaconWork *pWork);
static BOOL func_ov010_02157724(DeliveryBeaconWork *pWork);
static void func_ov010_02157760(void *work);
static void func_ov010_02157818(DeliveryBeaconWork *pWork);
static void func_ov010_02157870(void *work);
static void func_ov010_0215788c(void *work);

static const NetCommand data_ov010_02170040[] = {
    {func_ov010_02157660, NULL},
};

static GFLNetInitData data_ov010_02172780 = {
    data_ov010_02170040,
    NELEMS(data_ov010_02170040),
    func_ov010_0215765c,
    func_ov010_02157664,
    NULL,
    NULL,
    func_ov010_0215766c,
    func_ov010_021576a8,
    func_ov010_021576ac,
    NULL,
    NULL,
    func_ov010_02157668,
    NULL,
    {0},
    NULL,
    NULL,
    0,
    {1, 0, 0, 0, 0x80, 0x13, 0, 0},
    HEAPID_USER,
    0xd,
    0xf,
    0xd,
    0xf0,
    0,
    1,
    0x58,
    0x10,
    1,
    0,
    0,
    1,
    5,
    {0x2c, 1, 0, 0},
    0x1f4,
    0,
};

static void func_ov010_0215765c(void *work, int netId) {
}

static void func_ov010_02157660(int netId, int size, const void *data, void *work, NetHandle *handle) {
}

static void func_ov010_02157664(void *work, int netId) {
}

static void func_ov010_02157668(void *work) {
}

// The beacon to send
static void *func_ov010_0215766c(void *work) {
    DeliveryBeaconWork *pWork = work;
    int index = pWork->sendIndex % DELIVERY_BEACON_MAX_NUM;
    int dataIndex = pWork->sendIndex / DELIVERY_BEACON_MAX_NUM;

    if (dataIndex >= pWork->aInit.dataNum) {
        pWork->sendIndex = 0;
        dataIndex = 0;
    }
    return &pWork->beacons[dataIndex][index];
}

static int func_ov010_021576a8(void *work) {
    return sizeof(DeliveryBeaconData);
}

static BOOL func_ov010_021576ac(u32 a, u32 b) {
    if (a == b) {
        return TRUE;
    }
    return FALSE;
}

static void func_ov010_021576b8(DeliveryBeaconWork *pWork, DeliveryBeaconSeq seq) {
    pWork->seq = seq;
}

static void func_ov010_021576c4(DeliveryBeaconWork *pWork, DeliveryBeaconSeq seq, u32 line) {
    func_ov010_021576b8(pWork, seq);
}

static void func_ov010_021576cc(void *work) {
}

void *func_ov010_021576d0(const DeliveryInit *init) {
    DeliveryBeaconWork *pWork =
        GFL_HeapAllocate(init->heapId, sizeof(DeliveryBeaconWork), TRUE, "delivery_beacon.c", 295);

    MI_CpuCopy8(init, &pWork->aInit, sizeof(DeliveryInit));
    return pWork;
}

// File metadata for failed assertions.
// pokebw2's sDeliveryBeaconAssertFileName
static char data_ov010_021727f0[] = "";

static void func_ov010_02157700(DeliveryBeaconWork *pWork) {
    if (pWork->aInit.data[0].datasize != 0) {
        if (!(pWork->aInit.data[0].datasize < DELIVERY_BEACON_MAX_NUM * DELIVERY_BEACON_ONCE_NUM)) {
            GFL_DebugAssertFail(data_ov010_021727f0, 0, data_ov010_021727f0);
        }
    }
}

// Whether every beacon has come
static BOOL func_ov010_02157724(DeliveryBeaconWork *pWork) {
    int i;
    int count = pWork->beacons[0][0].count;

    if (count == 0) {
        return FALSE;
    }
    for (i = 0; i < count; i++) {
        if (count != pWork->beacons[0][i].count) {
            return FALSE;
        }
    }
    return TRUE;
}

// Takes the beacons that have come
static void func_ov010_02157760(void *work) {
    DeliveryBeaconWork *pWork = work;
    int i;
    int index;
    DeliveryBeaconData *beacon;
    u16 crc;

    for (i = 0; i < 16; i++) {
        beacon = func_ov014_02177b2c(i);
        if (beacon == NULL) {
            continue;
        }
        if (beacon->region != pWork->aInit.data[0].region) {
            func_ov014_02177bac(i);
        } else if ((beacon->mask & pWork->aInit.data[0].mask) == 0) {
            func_ov014_02177bac(i);
        } else if (beacon->unk5E != pWork->unk5E && pWork->unk5E != 0xff) {
            func_ov014_02177bac(i);
        } else {
            index = beacon->index - 1;
            if (index < DELIVERY_BEACON_MAX_NUM && pWork->beacons[0][index].index == 0) {
                crc = getCRC16(beacon->data, DELIVERY_BEACON_ONCE_NUM);
                if (crc != beacon->crc) {
                    func_ov014_02177bac(i);
                } else {
                    if (pWork->unk5E == 0xff) {
                        pWork->unk5E = beacon->unk5E;
                    }
                    func_ov014_021780c4(i);
                    MI_CpuCopy8(beacon, &pWork->beacons[0][index], sizeof(DeliveryBeaconData));
                    func_ov014_02177bcc();
                }
            }
        }
    }
}

// Decrypts the beacons' data into the buffer
static void func_ov010_02157818(DeliveryBeaconWork *pWork) {
    int i;
    DeliveryBeaconData *beacon;
    u16 pos;

    for (i = 0; i < DELIVERY_BEACON_MAX_NUM; i++) {
        pos = i * DELIVERY_BEACON_ONCE_NUM;
        beacon = &pWork->beacons[0][i];
        decryptData(beacon->data, DELIVERY_BEACON_ONCE_NUM, beacon->seed);
        if (pWork->aInit.data[0].datasize > pos + DELIVERY_BEACON_ONCE_NUM) {
            MI_CpuCopy8(beacon->data, pWork->aInit.data[0].pData + i * DELIVERY_BEACON_ONCE_NUM,
                       DELIVERY_BEACON_ONCE_NUM);
        } else {
            MI_CpuCopy8(beacon->data, pWork->aInit.data[0].pData + i * DELIVERY_BEACON_ONCE_NUM,
                       pWork->aInit.data[0].datasize - pos);
        }
    }
}

static void func_ov010_02157870(void *work) {
    func_0203d010();
    func_ov010_021576c4(work, func_ov010_02157760, 471);
}

static void func_ov010_0215788c(void *work) {
    func_ov010_021576c4(work, func_ov010_02157870, 477);
}

BOOL func_ov010_021578a0(void *work) {
    DeliveryBeaconWork *pWork = work;

    if (func_0203cd98()) {
        return FALSE;
    }
    data_ov010_02172780.gameCommandBase = pWork->aInit.code;
    pWork->unk5E = 0xff;
    func_ov010_02157700(pWork);
    func_0203cbac(&data_ov010_02172780, func_ov010_0215788c, pWork);
    func_ov010_021576c4(pWork, func_ov010_021576cc, 498);
    return TRUE;
}

BOOL func_ov010_021578f0(void *work) {
    DeliveryBeaconWork *pWork = work;
    int i;

    for (i = 0; i < DELIVERY_BEACON_MAX_NUM; i++) {
        if (pWork->beacons[0][i].count != 0) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL func_ov010_02157910(void *work) {
    BOOL done = func_ov010_02157724(work);

    if (done) {
        func_ov010_02157818(work);
    }
    return done;
}

void func_ov010_02157928(void *work) {
    DeliveryBeaconWork *pWork = work;

    pWork->seq(pWork);
}

void func_ov010_02157938(void *work) {
    GFL_HeapFree(work);
    func_0203ceac(NULL);
}
