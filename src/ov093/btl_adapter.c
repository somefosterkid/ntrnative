// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov167/btl_adapter.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "battle/btl_adapter.h"
#include "battle/btl_net.h"
#include "gfl/heap.h"
#include "gfl/std.h"

// A command and its data, as the server sends it to a client
typedef struct {
    u16 cmd;
    u16 size;
    u8 data[0xbbc];
} BtlAdapterCmd;

struct BtlAdapter {
    void *netHandle;
    // The client's reply to the last command, and its size
    void *returnData;
    u32 returnSize;
    // 1 once the reply has arrived
    u8 returned : 4;
    // The commands go over the link
    u8 isNet : 4;
    u8 clientId;
    // Where the last command is in its exchange: 0 idle, 1 to send, 3 sent and waiting for the reply, 4 replied
    u8 state;
    u8 cmd;
    BtlAdapterCmd sendCmd;
};

static BOOL func_ov093_021eeb84(BtlAdapter *adapter);
static BOOL func_ov093_021eeba4(BtlAdapter *adapter);

// The battle's communication mode (BtlFieldSituation.unk18): while it is 0, the server and the client share one adapter
// per client ID; otherwise each caller gets its own
// pokebw2's sCommMode
static u32 data_ov093_021f6384;
// pokebw2's sAdapters
static BtlAdapter *data_ov093_021f6388[4];

void func_ov093_021eea30(u8 commMode) {
    int i;

    for (i = 0; i < 4; i++) {
        data_ov093_021f6388[i] = NULL;
    }
    data_ov093_021f6384 = commMode;
}

void func_ov093_021eea54(void) {
}

BtlAdapter *func_ov093_021eea58(void *netHandle, u8 clientId, BOOL flag, HeapID heapId) {
    BtlAdapter *adapter = NULL;

    if (data_ov093_021f6388[clientId] == NULL) {
        adapter = GFL_HeapAllocate(heapId, sizeof(BtlAdapter), TRUE, "btl_adapter.c", 0x7e);
        adapter->netHandle = netHandle;
        adapter->clientId = clientId;
        adapter->isNet = flag;
        adapter->state = 0;
        adapter->cmd = 0;
        adapter->returned = 0;
    }
    if (data_ov093_021f6384 == 0) {
        if (data_ov093_021f6388[clientId] == NULL) {
            data_ov093_021f6388[clientId] = adapter;
        }
        return data_ov093_021f6388[clientId];
    }
    data_ov093_021f6388[clientId] = adapter;
    return adapter;
}

void func_ov093_021eead0(BtlAdapter *adapter) {
    adapter->state = 0;
    adapter->cmd = 0;
    adapter->returned = 0;
}

void func_ov093_021eeae0(BtlAdapter *adapter) {
    GFL_HeapFree(adapter);
}

void func_ov093_021eeae8(void) {
    if (data_ov093_021f6384 != 0) {
        func_ov093_021d4c58();
    }
}

void func_ov093_021eeafc(void) {
}

void func_ov093_021eeb00(BtlAdapter *adapter, u32 cmd, const void *data, u32 size) {
    adapter->state = 1;
    adapter->cmd = cmd;
    adapter->sendCmd.cmd = cmd;
    adapter->sendCmd.size = size;
    MI_CpuCopy8(data, adapter->sendCmd.data, size);
    adapter->returnData = NULL;
    adapter->returnSize = 0;
    adapter->returned = 0;
}

BOOL func_ov093_021eeb2c(BtlAdapter *adapter) {
    switch (adapter->state) {
    case 1:
        if (!func_ov093_021eeb84(adapter)) {
            break;
        }
        adapter->state = 3;
        // fallthrough
    case 3:
        if (!func_ov093_021eeba4(adapter)) {
            break;
        }
        adapter->state = 4;
        // fallthrough
    case 4:
        return TRUE;
    }
    return FALSE;
}

void *func_ov093_021eeb64(BtlAdapter *adapter, u32 *size) {
    if (size != NULL) {
        *size = adapter->returnSize;
    }
    return adapter->returnData;
}

void func_ov093_021eeb70(BtlAdapter *adapter) {
    adapter->state = 0;
    if (adapter->isNet) {
        func_ov093_021d4c58();
    }
}

static BOOL func_ov093_021eeb84(BtlAdapter *adapter) {
    if (adapter->isNet) {
        BtlAdapterCmd *cmd = &adapter->sendCmd;
        return func_ov093_021d4b58(adapter->clientId, cmd, cmd->size + 4);
    }
    return TRUE;
}

static BOOL func_ov093_021eeba4(BtlAdapter *adapter) {
    if (adapter->isNet) {
        if (func_ov093_021d4b98()) {
            adapter->returnSize = func_ov093_021d4bd4(adapter->clientId, &adapter->returnData);
            adapter->returned = 1;
            return TRUE;
        }
        return FALSE;
    }
    return adapter->returned;
}

void func_ov093_021eebdc(BtlAdapter *adapter) {
    adapter->sendCmd.cmd = 0;
    adapter->sendCmd.size = 4;
}

u8 func_ov093_021eebe8(BtlAdapter *adapter) {
    if (adapter->isNet) {
        if (func_ov093_021d4c60()) {
            void *buf;
            BtlAdapterCmd *recv;
            func_ov093_021d4c6c(&buf);
            recv = buf;
            return recv->cmd;
        }
        return 0;
    }
    if (adapter->returned == 0) {
        return adapter->sendCmd.cmd;
    }
    return 0;
}

u16 func_ov093_021eec20(BtlAdapter *adapter, const void **data) {
    if (adapter->isNet) {
        void *buf;
        BtlAdapterCmd *recv;
        func_ov093_021d4c6c(&buf);
        recv = buf;
        *data = recv->data;
        return recv->size;
    }
    *data = adapter->sendCmd.data;
    return adapter->sendCmd.size;
}

BOOL func_ov093_021eec4c(BtlAdapter *adapter, const void *data, u32 size) {
    if (adapter->isNet) {
        return func_ov093_021d4c84(data, size);
    }
    if (!adapter->returned) {
        adapter->returnData = data;
        adapter->returnSize = size;
        adapter->returned = 1;
    }
    return TRUE;
}
