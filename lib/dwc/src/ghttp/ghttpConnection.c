// Black's code at ov114 0x021c08b0-0x021c109c. Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
// (the same functions in the same order); the name is that file's.

#include <string.h>
#include <stdio.h>
#include "types.h"
#include "dwc/dwc.h"
#include "dwc/ext.h"
#include "dwc/nhttp.h"
#include "dwc/base.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "nitro/std.h"
#include "nitrowifi/cps.h"
#include "nitrowifi/soc.h"
#include "field/unity_tower.h"
#include "save/player_info.h"
#include "struct_decls.h"
#include "system/str_tool.h"
#include "gs/nonport.h"
#include "gs/darray.h"
#include "nitro/os.h"
#include "gfl/blact.h"
#include "dwc/ghttp_internal.h"

typedef struct GHIConnectionList {
    s32 count;
    s32 active;
    s32 nextId;
    s32 **connections;
} GHIConnectionList;

static GHIConnectionList data_ov114_021d3b30;
#pragma thumb off
s32 func_ov114_021c08b0(void) {
    s32 i;
    s32 count;
    s32 newCount;
    s32 **connections;

    for (i = 0; i < data_ov114_021d3b30.count; i++) {
        if (data_ov114_021d3b30.connections[i][0] == 0) {
            return i;
        }
    }

    count = data_ov114_021d3b30.count;
    newCount = count + 4;
    connections = gsirealloc(data_ov114_021d3b30.connections, newCount * 4);
    if (connections == NULL) {
        return -1;
    }
    data_ov114_021d3b30.connections = connections;
    for (i = count; i < newCount; i++) {
        data_ov114_021d3b30.connections[i] = gsimalloc(0x1d4);
        if (data_ov114_021d3b30.connections[i] == NULL) {
            for (i--; i >= count; i--) {
                gsifree(data_ov114_021d3b30.connections[i]);
            }
            return -1;
        }
        data_ov114_021d3b30.connections[i][0] = 0;
    }
    data_ov114_021d3b30.count = newCount;
    return count;
}

GHIConnection *func_ov114_021c0998(void) {
    s32 slot;
    GHIConnection *connection;
    s32 ok;

    func_ov114_021c0370();
    slot = func_ov114_021c08b0();
    if (slot == -1) {
        func_ov114_021c0374();
        return NULL;
    }
    connection = (GHIConnection *)data_ov114_021d3b30.connections[slot];
    memset(connection, 0, sizeof(GHIConnection));
    connection->inUse = TRUE;
    connection->request = slot;
    connection->uniqueId = data_ov114_021d3b30.nextId++;
    connection->type = 0;
    connection->state = 0;
    connection->url = NULL;
    connection->serverAddress = NULL;
    connection->serverIp = 0;
    connection->serverPort = 0;
    connection->requestPath = NULL;
    connection->headers = NULL;
    connection->unk_30 = 0;
    connection->blocking = FALSE;
    connection->keepAlive = FALSE;
    connection->result = 0;
    connection->progressCallback = NULL;
    connection->completedCallback = NULL;
    connection->callbackParam = NULL;
    connection->socket = -1;
    connection->socketError = 0;
    connection->userBuffer = FALSE;
    connection->httpMajor = 0;
    connection->httpMinor = 0;
    connection->statusCode = 0;
    connection->statusTextPos = 0;
    connection->headersPos = 0;
    connection->completed = FALSE;
    connection->bytesReceived = 0;
    connection->totalSize = -1;
    connection->redirectUrl = NULL;
    connection->redirectCount = 0;
    connection->chunked = FALSE;
    connection->processing = FALSE;
    connection->throttle = 0;
    connection->lastThrottleTime = 0;
    connection->post = NULL;
    connection->maxRecvTime = 500;
    connection->proxyPort = 80;
    connection->proxyAddress = NULL;
    connection->encryptor.state = NULL;
    connection->encryptor.open = FALSE;
    connection->lastRecvTime = 0;
    connection->hostLookup = NULL;

    ok = func_ov114_021bfbd0((s32)connection, (s32 *)&connection->sendBuffer, 0x800, 0x1000);
    if (ok) {
        ok = func_ov114_021bfbd0((s32)connection, (s32 *)&connection->encodeBuffer, 0x800, 0x400);
    }
    if (ok) {
        ok = func_ov114_021bfbd0((s32)connection, (s32 *)&connection->recvBuffer, 0x800, 0x800);
    }
    if (ok) {
        ok = func_ov114_021bfbd0((s32)connection, (s32 *)&connection->decodeBuffer, 0x800, 0x400);
    }
    if (!ok) {
        func_ov114_021c0d40((s32 *)connection);
        func_ov114_021c0374();
        return NULL;
    }
    data_ov114_021d3b30.active++;
    func_ov114_021c0374();
    return connection;
}

u32 func_ov114_021c0b4c(s32 *param_1) {
    if (param_1 == NULL) {
        return 0;
    }
    if (param_1[0] == 0) {
        return 0;
    }
    if (param_1[1] < 0) {
        return 0;
    }
    if (param_1[1] >= data_ov114_021d3b30.count) {
        return 0;
    }
    func_ov114_021c0370();
    if (param_1[0x73] != 0) {
        gsDebugFormat(8, 3, 0x10, "Cancelling Thread and freeing memory\n");
        func_ov009_0217f1c0((void *)param_1[0x73]);
        param_1[0x73] = 0;
    }
    if (param_1[0x14] != -1) {
        shutdown(param_1[0x14], 2);
        if (func_ov009_0217eb40(param_1[0x14], &param_1[0x72]) == 0) {
            func_ov114_021c0374();
            return 0;
        }
        param_1[0x14] = -1;
    }
    func_ov114_021c0374();
    return 1;
}

u32 func_ov114_021c0c14(GHIConnection *connection) {
    if (connection == NULL) {
        return 0;
    }
    if (!connection->inUse) {
        return 0;
    }
    if (connection->request < 0) {
        return 0;
    }
    if (connection->request >= data_ov114_021d3b30.count) {
        return 0;
    }

    func_ov114_021c0370();
    gsifree(connection->url);
    connection->url = NULL;
    gsifree(connection->unk_34);
    connection->unk_34 = NULL;
    gsifree(connection->serverAddress);
    connection->serverAddress = NULL;
    gsifree(connection->requestPath);
    connection->requestPath = NULL;
    gsifree(connection->headers);
    connection->headers = NULL;
    gsifree(connection->redirectUrl);
    connection->headers = NULL;
    gsifree(connection->proxyAddress);
    connection->proxyAddress = NULL;
    func_ov114_021bfcb4((s32 *)&connection->sendBuffer);
    func_ov114_021bfcb4((s32 *)&connection->encodeBuffer);
    func_ov114_021bfcb4((s32 *)&connection->recvBuffer);
    func_ov114_021bfcb4((s32 *)&connection->decodeBuffer);
    func_ov114_021bfcb4((s32 *)&connection->fileBuffer);
    if (connection->postingState.states != NULL) {
        func_ov114_021c2bb0(connection);
    }
    if (connection->encryptor.open) {
        if (connection->encryptor.cleanup != NULL) {
            connection->encryptor.cleanup(connection, &connection->encryptor);
        }
        connection->encryptor.open = FALSE;
    }
    connection->lastRecvTime = 0;
    connection->inUse = FALSE;
    data_ov114_021d3b30.active--;
    func_ov114_021c0374();
    return 1;
}

u32 func_ov114_021c0d40(s32 *param_1) {
    u32 result;

    if (param_1 == NULL) {
        return 0;
    }
    if (param_1[0] == 0) {
        return 0;
    }
    if (param_1[1] < 0) {
        return 0;
    }
    if (param_1[1] >= data_ov114_021d3b30.count) {
        return 0;
    }
    result = func_ov114_021c0b4c(param_1);
    if (result != 1) {
        return 0;
    }
    result = func_ov114_021c0c14((GHIConnection *)param_1);
    return result;
}

s32 *func_ov114_021c0da4(s32 param_1) {
    s32 *node;

    func_ov114_021c0370();
    if (param_1 < 0 || param_1 >= data_ov114_021d3b30.count) {
        func_ov114_021c0374();
        return NULL;
    }
    node = (s32 *)data_ov114_021d3b30.connections[param_1];
    if (node[0] == 0) {
        node = NULL;
    }
    func_ov114_021c0374();
    return node;
}

void func_ov114_021c0df8(u8 *param_1) {
    s32 i;

    if (data_ov114_021d3b30.active <= 0) {
        return;
    }
    func_ov114_021c0370();
    for (i = 0; i < data_ov114_021d3b30.count; i++) {
        s32 *node = (s32 *)data_ov114_021d3b30.connections[i];
        if (node[0] != 0) {
            ((void (*)(s32 *))param_1)(node);
        }
    }
    func_ov114_021c0374();
}

void func_ov114_021c0e58(void *arg0) {
    s32 result;

    gsDebugFormat(8, 3, 0x10, (s8 *)"Redirecting Connection\n");
    if (*(s32 *)((s32)(u8 *)arg0 + 0x1cc) != 0) {
        gsDebugFormat(8, 3, 0x10, "Cancelling Thread and freeing memory\n");
        func_ov009_0217f1c0(*(void **)((s32)(u8 *)arg0 + 0x1cc));
        *(u32 *)((s32)(u8 *)arg0 + 0x1cc) = 0;
    }
    shutdown(*(s32 *)((s32)(u8 *)arg0 + 0x50), 2);
    result = func_ov009_0217eb40(*(s32 *)((s32)(u8 *)arg0 + 0x50), (s32 *)((s32)(u8 *)arg0 + 0x1c8));
    if (result != 0) {
        gsifree(*(void **)((s32)(u8 *)arg0 + 0x14));
        *(u32 *)((s32)(u8 *)arg0 + 0x14) = *(u32 *)((s32)(u8 *)arg0 + 0x130);
        *(u32 *)((s32)(u8 *)arg0 + 0x130) = 0;
        gsifree(*(void **)((s32)(u8 *)arg0 + 0x18));
        *(u32 *)((s32)(u8 *)arg0 + 0x18) = 0;
        *(u32 *)((s32)(u8 *)arg0 + 0x1c) = 0;
        *(u16 *)((s32)(u8 *)arg0 + 0x20) = 0;
        gsifree(*(void **)((s32)(u8 *)arg0 + 0x24));
        *(u32 *)((s32)(u8 *)arg0 + 0x24) = 0;
        *(u32 *)((s32)(u8 *)arg0 + 0x10) = 0;
        *(u32 *)((s32)(u8 *)arg0 + 0x50) = 0xffffffff;
        func_ov114_021c00fc((s32 *)((s32)(u8 *)arg0 + 0x58));
        func_ov114_021c00fc((s32 *)((s32)(u8 *)arg0 + 0x7c));
        func_ov114_021c00fc((s32 *)((s32)(u8 *)arg0 + 0xa0));
        func_ov114_021c00fc((s32 *)((s32)(u8 *)arg0 + 0xc4));
        *(u32 *)((s32)(u8 *)arg0 + 0x110) = 0;
        *(u32 *)((s32)(u8 *)arg0 + 0x114) = 0;
        *(u32 *)((s32)(u8 *)arg0 + 0x118) = 0;
        *(u32 *)((s32)(u8 *)arg0 + 0x11c) = 0;
        *(u32 *)((s32)(u8 *)arg0 + 0x120) = 0;
        *(u32 *)((s32)(u8 *)arg0 + 0x158) = 0;
        if (*(s32 *)((s32)(u8 *)arg0 + 0x19c) != 0) {
            if (*(void (**)())((s32)(u8 *)arg0 + 0x1b8) != NULL) {
                (**(void (**)())((s32)(u8 *)arg0 + 0x1b8))(arg0, (s32)(u8 *)arg0 + 0x194);
            }
            *(u32 *)((s32)(u8 *)arg0 + 0x19c) = 0;
            result = strncmp("https://", *(s8 **)((s32)(u8 *)arg0 + 0x14), 8);
            if (result != 0) {
                *(u32 *)((s32)(u8 *)arg0 + 0x198) = 0;
                *(u32 *)((s32)(u8 *)arg0 + 0x194) = 0;
            }
        }
        *(s32 *)((s32)(u8 *)arg0 + 0x134) = *(s32 *)((s32)(u8 *)arg0 + 0x134) + 1;
        return;
    }
}

void func_ov114_021c0fb0(void) {
    s32 i;

    if (data_ov114_021d3b30.connections == NULL) {
        return;
    }
    func_ov114_021c0df8((u8 *)func_ov114_021c0d40);
    i = 0;
    if (0 < data_ov114_021d3b30.count) {
        do {
            gsifree(data_ov114_021d3b30.connections[i]);
            i = i + 1;
        } while (i < data_ov114_021d3b30.count);
    }
    gsifree(data_ov114_021d3b30.connections);
    data_ov114_021d3b30.connections = NULL;
    data_ov114_021d3b30.count = 0;
    data_ov114_021d3b30.active = 0;
}

s8 *func_ov114_021c1020(s8 *param_1) {
    s8 *p;
    s32 n;
    s8 saved;

    if (strncmp(param_1, "http://", 7) == 0) {
        p = param_1 + 7;
    } else if (strncmp(param_1, "https://", 8) == 0) {
        p = param_1 + 8;
    } else {
        return NULL;
    }
    n = strcspn(p, ":/");
    saved = p[n];
    p[n] = 0;
    func_ov009_0217f1e0(p);
    p[n] = saved;
}
