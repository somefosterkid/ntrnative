// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/net_lower_data.c.
#include "types.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_handle.h"
#include "gfl/net_lower_data.h"
#include "gfl/net_system.h"
#include "gfl/std.h"

enum {
    NET_LDATA_HEADER_START,
    NET_LDATA_HEADER_END,
    NET_LDATA_HEADER_2,
};

// The header command, which starts and ends the data
typedef struct {
    u32 type : 2;
    u32 size : 30;
} NetLdataHeader;

// Where a machine's data is received
typedef struct {
    void *buffer;
    u32 bufferSize;
    u32 size;
    u32 recvSize;
    u8 done : 1;
    // Whether buffer is freed with this
    u8 allocated : 1;
    u8 unk11[0x37];
} NetLdataPost;

typedef struct {
    NetLdataPost *postWork[GFL_NET_MACHINE_MAX];
    void *sendData;
    u32 sendSize;
    u8 dest;
    BOOL fromServer;
    BOOL sendDone;
    // Whether the start header still has to be sent
    BOOL sendStart;
    BOOL unk38;
    BOOL unk3C;
    BOOL unk40;
    u32 sentSize;
} NetLdataWork;

static NetLdataWork *netLdataWork;

static BOOL func_0203ec04(int size);
static BOOL func_0203ed84(int type);

void func_0203e5f8(HeapID heapId, BOOL a1) {
    u8 i;

    GFL_ASSERT_AT(101, netLdataWork == NULL);
    netLdataWork = GFL_HeapAllocate(heapId, sizeof(NetLdataWork), TRUE, "net_lower_data.c", 103);
    netLdataWork->sendData = NULL;
    netLdataWork->sendSize = 0;
    netLdataWork->sentSize = 0;
    netLdataWork->sendDone = FALSE;
    netLdataWork->sendStart = FALSE;
    netLdataWork->unk38 = a1;
    netLdataWork->unk3C = TRUE;
    netLdataWork->unk40 = FALSE;
    for (i = 0; i < GFL_NET_MACHINE_MAX; i++) {
        netLdataWork->postWork[i] = NULL;
    }
}

void func_0203e6c0(void) {
    u8 i;

    GFL_ASSERT_AT(129, netLdataWork != NULL);
    for (i = 0; i < GFL_NET_MACHINE_MAX; i++) {
        if (netLdataWork->postWork[i] != NULL) {
            if (netLdataWork->postWork[i]->allocated == TRUE) {
                GFL_HeapFree(netLdataWork->postWork[i]->buffer);
            }
            GFL_HeapFree(netLdataWork->postWork[i]);
        }
    }
    GFL_HeapFree(netLdataWork);
    netLdataWork = NULL;
}

void func_0203e758(void *data, u32 size, int dest, BOOL fromServer) {
    GFL_ASSERT_AT(159, netLdataWork != NULL);
    GFL_ASSERT_AT(160, netLdataWork->sendData == NULL);
    netLdataWork->sendData = data;
    netLdataWork->sendSize = size;
    netLdataWork->dest = dest;
    netLdataWork->fromServer = fromServer;
    netLdataWork->sentSize = 0;
    netLdataWork->sendStart = TRUE;
    netLdataWork->sendDone = FALSE;
}

void func_0203e808(u32 bufferSize, int netId, HeapID heapId, void *buffer) {
    GFL_ASSERT_AT(207, netLdataWork != NULL);
    GFL_ASSERT_AT(208, netLdataWork->postWork[netId] == NULL);
    netLdataWork->postWork[netId] = GFL_HeapAllocate(heapId, sizeof(NetLdataPost), TRUE, "net_lower_data.c", 210);
    netLdataWork->postWork[netId]->buffer = buffer;
    netLdataWork->postWork[netId]->bufferSize = bufferSize;
    netLdataWork->postWork[netId]->allocated = FALSE;
}

void func_0203e8c8(int netId) {
    GFL_ASSERT_AT(226, netLdataWork != NULL);
    GFL_ASSERT_AT(227, netLdataWork->postWork[netId] != NULL);
    if (netLdataWork->postWork[netId]->allocated == TRUE) {
        GFL_HeapFree(netLdataWork->postWork[netId]->buffer);
    }
    GFL_HeapFree(netLdataWork->postWork[netId]);
    netLdataWork->postWork[netId] = NULL;
}

void *func_0203e96c(int netId) {
    GFL_ASSERT_AT(246, netLdataWork != NULL);
    GFL_ASSERT_AT(247, netLdataWork->postWork[netId] != NULL);
    return netLdataWork->postWork[netId]->buffer;
}

u32 func_0203e9dc(int netId) {
    GFL_ASSERT_AT(261, netLdataWork != NULL);
    GFL_ASSERT_AT(262, netLdataWork->postWork[netId] != NULL);
    return netLdataWork->postWork[netId]->recvSize;
}

BOOL func_0203ea54(void) {
    if (netLdataWork == NULL) {
        return FALSE;
    }
    return netLdataWork->sendDone;
}

BOOL func_0203ea70(int netId) {
    if (netLdataWork == NULL || netLdataWork->postWork[netId] == NULL) {
        return FALSE;
    }
    return netLdataWork->postWork[netId]->done;
}

void func_0203ea9c(void) {
    if (netLdataWork == NULL || netLdataWork->sendData == NULL || func_0203cd98() != TRUE) {
        return;
    }
    if (netLdataWork->sendStart == TRUE) {
        if (func_0203ed84(NET_LDATA_HEADER_START) == TRUE) {
            netLdataWork->sendStart = FALSE;
        }
    } else if (netLdataWork->sentSize < netLdataWork->sendSize) {
        if (netLdataWork->unk3C == TRUE) {
            GFLNetInitData *ini = func_0203d798();
            int size;

            // What fits in the packet after what is already queued
            if (netLdataWork->fromServer == TRUE) {
                size = ini->unk6C - func_02038d4c();
            } else {
                size = ini->maxSendSize - func_02038d30();
            }
            size -= 16;
            if (size > 16) {
                if (netLdataWork->sentSize + size > netLdataWork->sendSize) {
                    size = netLdataWork->sendSize - netLdataWork->sentSize;
                }
                if (func_0203ec04(size) == TRUE) {
                    netLdataWork->sentSize += size;
                    if (netLdataWork->unk38 == TRUE) {
                        netLdataWork->unk3C = FALSE;
                    }
                }
            }
        }
    } else if (netLdataWork->sendDone == FALSE) {
        if (func_0203ed84(NET_LDATA_HEADER_END) == TRUE) {
            netLdataWork->sendDone = TRUE;
            netLdataWork->sendData = NULL;
        }
    }
    if (netLdataWork->unk40 == TRUE) {
        if (func_0203ed84(NET_LDATA_HEADER_2) == TRUE) {
            netLdataWork->unk40 = FALSE;
        }
    }
}

static BOOL func_0203ec04(int size) {
    void *data = (u8 *)netLdataWork->sendData + netLdataWork->sentSize;
    NetHandle *handle;

    if (netLdataWork->fromServer == TRUE) {
        handle = func_0203996c(GFL_NET_NETID_SERVER);
    } else {
        handle = func_020399a8();
    }
    return func_0203d4d0(handle, netLdataWork->dest, 14, size, data, 0, TRUE, TRUE);
}

void *func_0203ec7c(int netId, void *work, int size) {
    GFLNetInitData *ini;

    if (netId == GFL_NET_NETID_SERVER) {
        netId = 0;
    }
    if (netLdataWork != NULL && netLdataWork->postWork[netId] != NULL) {
        ini = func_0203d798();
        if (netLdataWork == NULL || netLdataWork->postWork[netId] == NULL) {
            return NULL;
        }
        return (u8 *)netLdataWork->postWork[netId]->buffer + netLdataWork->postWork[netId]->recvSize;
    }
    return NULL;
}

void func_0203ecd8(int netId, int size, const void *data, void *work, NetHandle *handle) {
    GFLNetInitData *ini;

    if (netId == GFL_NET_NETID_SERVER) {
        netId = 0;
    }
    if (netLdataWork == NULL || netLdataWork->postWork[netId] == NULL) {
        return;
    }
    ini = func_0203d798();
    if (netLdataWork == NULL || netLdataWork->postWork[netId] == NULL) {
        return;
    }
    if (handle != func_020399a8() && !ini->bMPMode) {
        return;
    }
    netLdataWork->postWork[netId]->recvSize += size;
    if (netLdataWork->unk38 == TRUE && netId != func_0203d1a4(func_020399a8())) {
        netLdataWork->unk40 = TRUE;
    }
}

static BOOL func_0203ed84(int type) {
    NetHandle *handle;
    NetLdataHeader header;

    if (netLdataWork->fromServer == TRUE) {
        handle = func_0203996c(GFL_NET_NETID_SERVER);
    } else {
        handle = func_020399a8();
    }
    header.type = type;
    header.size = netLdataWork->sendSize;
    return func_0203d4d0(handle, netLdataWork->dest, 15, sizeof(NetLdataHeader), &header, 0, TRUE, FALSE);
}

void func_0203ee18(int netId, int size, const void *data, void *work, NetHandle *handle) {
    const NetLdataHeader *header = data;
    GFLNetInitData *ini;

    if (netId == GFL_NET_NETID_SERVER) {
        netId = 0;
    }
    if (netLdataWork == NULL || netLdataWork->postWork[netId] == NULL) {
        return;
    }
    ini = func_0203d798();
    if (netLdataWork == NULL || netLdataWork->postWork[netId] == NULL) {
        return;
    }
    if (handle != func_020399a8() && !ini->bMPMode) {
        return;
    }
    switch (header->type) {
    case NET_LDATA_HEADER_START:
        GFL_ASSERT_MSG_AT(543, header->size <= netLdataWork->postWork[netId]->bufferSize,
                          "BufferSize is not enough!![%x][%x]\n", netLdataWork->postWork[netId]->bufferSize,
                          header->size);
        netLdataWork->postWork[netId]->size = header->size;
        netLdataWork->postWork[netId]->recvSize = 0;
        netLdataWork->postWork[netId]->done = FALSE;
        break;
    case NET_LDATA_HEADER_END:
        if (netLdataWork->postWork[netId]->recvSize == netLdataWork->postWork[netId]->size) {
            netLdataWork->postWork[netId]->done = TRUE;
        }
        break;
    case NET_LDATA_HEADER_2:
        if (netId != func_0203d1a4(func_020399a8())) {
            netLdataWork->unk3C = TRUE;
        }
        break;
    }
}
