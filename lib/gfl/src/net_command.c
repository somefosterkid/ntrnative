// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/net_command.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/net_irc_wireless.h"
#include "gfl/net_lower_data.h"
#include "gfl/net_state.h"
#include "gfl/net_system.h"
#include "gfl/std.h"

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
BOOL func_02038c18(int command, u8 *data, int size, u32 unused, int dest, int netId, BOOL noCopy);
int func_020395ec(NetHandle *handle);
NetHandle *func_020399a8(void);
void func_0203a388(int netID, int size, const void *data, void *work, NetHandle *handle);
void func_0203ee18(int netId, int size, const void *data, void *work, NetHandle *handle);
void MI_CpuCopy8(const void *src, void *dest, u32 size);

#define COMMAND_TABLE_MAX 4
#define HUGE_SEND_MAX 5
// The packet header and the command's header, which a chunk leaves room for
#define _HUGEDATA_DIFFBYTE 12

enum {
    NET_CMD_HUGE_DATA = 12,
    NET_CMD_HUGE_HEADER,
};

typedef struct {
    const NetCommand *list;
    int listNum;
    void *work;
    int base;
} NetCommandTable;

// Data in chunks being sent
typedef struct {
    int netId;
    u32 size;
    u8 *data;
    u16 command;
    u16 chunkSize;
    u16 lastSize;
    u16 chunksSent;
    u16 chunkCount;
    u8 netIds;
    // Whether chunks are being sent, and whether the header is to be sent
    u8 sending : 4;
    u8 headerPending : 4;
} NetHugeSend;

// What precedes data in chunks
typedef struct {
    u32 size;
    u16 chunkCount;
    u16 command;
    u16 chunkSize;
    u16 netIds;
} NetHugeHeader;

// Data in chunks arriving from a machine
typedef struct {
    u32 size;
    u8 *data;
    u16 command;
    u16 chunkSize;
    u16 chunksReceived;
    u16 chunkCount;
    u16 netIds;
    u8 used;
} NetHugeRecv;

// Black 2's; twinned code confirms the offsets of tables, hugeSends, aNetHugeRecv, unk220 and unk228 in Black
typedef struct {
    NetCommandTable tables[COMMAND_TABLE_MAX];
    NetHugeSend hugeSends[HUGE_SEND_MAX];
    NetHugeRecv aNetHugeRecv[GFL_NET_HANDLE_MAX];
    u8 unk16C[0xb4];
    u8 unk220[8];
    u8 unk228;
} NetCommandWork;

static NetCommandTable *func_0203a67c(int command);
static void *func_0203a70c(int command);
static void *func_0203a730(int command);
static void *func_0203a748(int command);
static int func_0203abc0(void);

static const NetCommand sBaseCommands[GFL_NET_CMD_BASE_COUNT] = {
    { (void *)func_0203a858, NULL },
    { func_0203be5c, NULL },
    { func_0203beac, NULL },
    { func_0203a020, NULL },
    { (void *)func_0203a150, NULL },
    { (void *)func_0203a27c, NULL },
    { func_0203c044, NULL },
    { func_02039b5c, NULL },
    { func_02039c2c, NULL },
    { func_0203e484, NULL },
    { func_0203a388, NULL },
    { func_0203c5c8, NULL },
    { func_0203a85c, NULL },
    { func_0203a958, NULL },
    { func_0203ecd8, func_0203ec7c },
    { func_0203ee18, NULL },
    { (void *)func_0203a858, NULL },
};

static NetCommandTable _BaseTable = { sBaseCommands, GFL_NET_CMD_BASE_COUNT };

// The name is in Black's assert texts
static NetCommandWork *_pCommandWork;

void func_0203a494(int base, const void *list, int listNum, void *work, HeapID unused) {
    int i;

    if (_pCommandWork == NULL) {
        _pCommandWork = GFL_HeapAllocate(HEAPID_SYSTEM, sizeof(NetCommandWork), TRUE, "net_command.c", 150);
    }
    func_0203a574(base, list, listNum, work);
    _BaseTable.work = work;
    for (i = 0; i < 8; i++) {
        _pCommandWork->unk220[i] = 0;
    }
    _pCommandWork->unk228 = 0;
}

void func_0203a53c(void) {
    if (_pCommandWork != NULL) {
        MI_CpuFill8(_pCommandWork, 0, sizeof(NetCommandWork));
        GFL_HeapFree(_pCommandWork);
        _pCommandWork = NULL;
    }
}

void func_0203a574(int base, const void *list, int listNum, void *work) {
    NetCommandWork *commandWork = _pCommandWork;

    if (commandWork != NULL) {
        int i;

        for (i = 0; i < COMMAND_TABLE_MAX; i++) {
            if (commandWork->tables[i].base == 0 || base == commandWork->tables[i].base) {
                commandWork->tables[i].base = base;
                _pCommandWork->tables[i].list = list;
                _pCommandWork->tables[i].listNum = listNum;
                _pCommandWork->tables[i].work = work;
                return;
            }
        }
    }
}

void func_0203a5e4(int base) {
    NetCommandWork *commandWork = _pCommandWork;

    if (commandWork != NULL) {
        int i;

        for (i = 0; i < COMMAND_TABLE_MAX; i++) {
            if (base == commandWork->tables[i].base) {
                MI_CpuFill8(&commandWork->tables[i], 0, sizeof(NetCommandTable));
                return;
            }
        }
    }
}

BOOL func_0203a634(int netIds) {
    if (netIds == GFL_NET_NETID_SERVER) {
        return TRUE;
    }
    if (netIds & (1 << func_02039338())) {
        return TRUE;
    }
    if (func_02039338() == 0 && (netIds & 0x80)) {
        return TRUE;
    }
    return FALSE;
}

static NetCommandTable *func_0203a67c(int command) {
    int i;
    int base = command & 0xff00;
    int co = (u8)command;
    NetCommandWork *commandWork;

    if (base == 0) {
        GFL_ASSERT_AT(0x10d, _BaseTable.listNum > co);
        return &_BaseTable;
    }
    commandWork = _pCommandWork;
    for (i = 0; i < COMMAND_TABLE_MAX; i++) {
        NetCommandTable *table = &commandWork->tables[i];

        if (base == table->base) {
            if (table->listNum > co) {
                return table;
            }
            return NULL;
        }
    }
    return NULL;
}

static void *func_0203a70c(int command) {
    NetCommandTable *table = func_0203a67c(command);

    if (table != NULL) {
        return table->list[command & 0xff].callback;
    }
    return NULL;
}

static void *func_0203a730(int command) {
    NetCommandTable *table = func_0203a67c(command);

    if (table != NULL) {
        return table->work;
    }
    return NULL;
}

static void *func_0203a748(int command) {
    NetCommandTable *table = func_0203a67c(command);

    if (table != NULL) {
        return table->list[command & 0xff].getBuffer;
    }
    return NULL;
}

void func_0203a770(int netId, int netIds, int command, int size, void *data, NetHandle *handle) {
    if (func_0203a634(netIds)) {
        BOOL ok = TRUE;
        void (*callback)(int netId, int size, const void *data, void *work, NetHandle *handle);

        if (!func_020399b8(handle) && command >= GFL_NET_CMD_BASE_COUNT) {
            ok = FALSE;
        }
        if (ok) {
            callback = func_0203a70c(command);
            if (callback != NULL) {
                callback(netId, size, data, func_0203a730(command), handle);
            }
        }
    }
}

BOOL func_0203a7ec(int command) {
    if (func_0203a70c(command) != NULL) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_0203a804(int command) {
    if (func_0203a748(command) != NULL) {
        return TRUE;
    }
    return FALSE;
}

void *func_0203a81c(int command, int netId, int size) {
    void *(*getBuffer)(int netId, void *work, int size) = func_0203a748(command);

    if (getBuffer != NULL) {
        return getBuffer(netId, func_0203a730(command), size);
    }
    return NULL;
}

void func_0203a858(void) {
}

void func_0203a85c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    int index = netId;

    if (netId == GFL_NET_NETID_SERVER) {
        index = GFL_NET_HANDLE_MAX - 1;
    }
    if (handle == func_020399a8() && _pCommandWork->aNetHugeRecv[index].used) {
        if (_pCommandWork->aNetHugeRecv[index].data != NULL) {
            MI_CpuCopy8(data,
                _pCommandWork->aNetHugeRecv[index].data
                    + _pCommandWork->aNetHugeRecv[index].chunksReceived * _pCommandWork->aNetHugeRecv[index].chunkSize,
                size);
        }
        _pCommandWork->aNetHugeRecv[index].chunksReceived++;
        if (_pCommandWork->aNetHugeRecv[index].chunksReceived == _pCommandWork->aNetHugeRecv[index].chunkCount) {
            if (_pCommandWork->aNetHugeRecv[index].data != NULL) {
                func_0203a770(netId, _pCommandWork->aNetHugeRecv[index].netIds,
                    _pCommandWork->aNetHugeRecv[index].command, _pCommandWork->aNetHugeRecv[index].size,
                    _pCommandWork->aNetHugeRecv[index].data, handle);
            }
            _pCommandWork->aNetHugeRecv[index].used = FALSE;
        }
    }
}

void func_0203a958(int netId, int size, const void *data, void *work, NetHandle *handle) {
    const NetHugeHeader *header = data;
    int index = netId;

    if (netId == GFL_NET_NETID_SERVER) {
        index = GFL_NET_HANDLE_MAX - 1;
    }
    if (handle == func_020399a8() && func_0203a81c(header->command, netId, header->size)) {
        GFL_ASSERT_AT(0x1ff, _pCommandWork->aNetHugeRecv[index].used==FALSE);
        _pCommandWork->aNetHugeRecv[index].used = TRUE;
        _pCommandWork->aNetHugeRecv[index].size = header->size;
        _pCommandWork->aNetHugeRecv[index].data = func_0203a81c(header->command, netId, header->size);
        GFL_ASSERT_AT(0x203, _pCommandWork->aNetHugeRecv[index].data);
        _pCommandWork->aNetHugeRecv[index].command = header->command;
        _pCommandWork->aNetHugeRecv[index].chunkSize = header->chunkSize;
        _pCommandWork->aNetHugeRecv[index].netIds = header->netIds;
        _pCommandWork->aNetHugeRecv[index].chunkCount = header->chunkCount;
        _pCommandWork->aNetHugeRecv[index].chunksReceived = 0;
    }
}

int func_0203aa94(NetHandle *handle, u8 netIds, u16 command, u32 size, void *data) {
    GFLNetInitData *pNetIni = func_0203d798();
    NetHugeSend *send;
    int i;

    for (i = 0; i < HUGE_SEND_MAX; i++) {
        if (_pCommandWork->hugeSends[i].headerPending == 0 && _pCommandWork->hugeSends[i].sending == 0) {
            break;
        }
    }
    if (i == HUGE_SEND_MAX) {
        return 0;
    }
    send = &_pCommandWork->hugeSends[i];
    send->size = size;
    GFL_ASSERT_AT(0x22b, (pNetIni->maxSendSize - _HUGEDATA_DIFFBYTE)>0);
    send->chunkSize = pNetIni->maxSendSize - _HUGEDATA_DIFFBYTE;
    send->chunkCount = size / send->chunkSize;
    send->lastSize = size % send->chunkSize;
    if (send->lastSize != 0) {
        send->chunkCount++;
    }
    send->chunksSent = 0;
    send->command = command;
    send->netIds = netIds;
    send->data = data;
    send->netId = func_020395ec(handle);
    send->headerPending = TRUE;
    return send->headerPending;
}

static int func_0203abc0(void) {
    int i;

    for (i = 0; i < HUGE_SEND_MAX; i++) {
        NetHugeSend *send = &_pCommandWork->hugeSends[i];

        if (send->headerPending) {
            NetHugeHeader header;

            header.chunkCount = send->chunkCount;
            header.size = send->size;
            header.command = send->command;
            header.chunkSize = send->chunkSize;
            header.netIds = send->netIds;
            send->sending = func_02038c18(NET_CMD_HUGE_HEADER, (u8 *)&header, sizeof(header), 1, send->netId,
                send->netIds, FALSE);
            send->headerPending = 1 - send->sending;
            return send->sending;
        }
    }
    return 0;
}

void func_0203acac(void) {
    int i;

    for (i = 0; i < HUGE_SEND_MAX; i++) {
        if (_pCommandWork->hugeSends[i].sending) {
            NetHugeSend *send = &_pCommandWork->hugeSends[i];
            u16 len = send->chunkSize;
            u32 offset;
            u8 *sendData = send->data;

            offset = len * send->chunksSent;

            // BUG: Data that is a whole number of chunks has no remainder, and its last chunk is sent empty
#ifdef BUGFIX
            if (send->chunksSent + 1 == send->chunkCount && send->lastSize != 0) {
#else
            if (send->chunksSent + 1 == send->chunkCount) {
#endif
                len = send->lastSize;
            }
            if (func_02038c18(NET_CMD_HUGE_DATA, sendData + offset, len, 1, send->netId, send->netIds, TRUE)) {
                send->chunksSent++;
            }
            if (send->chunksSent == send->chunkCount) {
                send->sending = FALSE;
            }
            return;
        }
    }
    func_0203abc0();
}
