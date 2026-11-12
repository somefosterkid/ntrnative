// Black's code at ov114 0x021bf04c-0x021bf520. Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
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
#include "dwc/nhttp_internal.h"

#pragma thumb off
// Makes a connection for a URL, a method and the buffer for the answer. The callback gets the events of the
// connection, with the argument
int func_ov114_021bf04c(const char *url, int method, void *buffer, u32 bufferSize, NHttpCallback callback,
                          void *arg) {
    NHttpRoot *root = func_ov114_021bef08();
    NHttpState *state = func_ov114_021bef3c(root);
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(root);
    NHttpConnection *connection = func_ov114_021ba820(sizeof(NHttpConnection), 0x20);

    if (connection == NULL) {
        func_ov114_021ba874(state, 1);
        return 0;
    }
    func_ov114_021ba13c(connection, sizeof(NHttpConnection));
    connection->request = func_ov114_021bb3cc(state, url, method, buffer, bufferSize, arg, 0, NULL);
    if (connection->request == NULL) {
        func_ov114_021ba850(connection);
        return 0;
    }
    connection->response = connection->request->response;
    connection->stage = NHTTP_STAGE_NONE;
    connection->callback = callback;
    connection->userBuffer = NULL;
    connection->userBufferSize = 0;
    connection->id = -1;
    func_ov114_021beb88(mutexInfo, connection);
    connection->status = 15;
    connection->unk_08 = 0;
    connection->running = TRUE;
    func_ov114_021bef60(connection, 0);
    connection->unk_30 = 0;
    connection->recvPos = 0;
    connection->recvSize = 0;
    func_ov114_021ba13c(connection->recvBuffer, NHTTP_RECV_BUFFER_SIZE);
    connection->waiters = 1;
    return (int)connection;
}

int func_ov114_021bf184(int handle) {
    NHttpConnection *connection =
        func_ov114_021bec00(func_ov114_021bef58(func_ov114_021bef08()), (NHttpConnection *)handle);

    if (connection == NULL) {
        return -1;
    }
    return func_ov114_021bf1b0(connection);
}

// Ends a connection once nobody uses it: cancels its request, waits for the thread to let go of it, and frees what it
// has
int func_ov114_021bf1b0(NHttpConnection *connection) {
    NHttpRoot *root = func_ov114_021bef08();
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(root);

    func_ov114_021bef50(root);
    if (connection == NULL) {
        return -1;
    }
    if (connection->waiters > 0 && connection->id >= 0) {
        func_ov114_021bba14(root, connection->id);
        connection->id = -1;
    }
    if (func_ov114_021bf018(mutexInfo, connection) > 0) {
        return 0;
    }
    if (connection->request != NULL && connection->request->sent == 0) {
        func_ov114_021bb8b4(root, connection->request);
    }
    if (connection->response != NULL) {
        if (connection->request != NULL) {
            func_ov114_021bf288(connection);
        }
        func_ov114_021bbb28(mutexInfo, connection->response);
    }
    func_ov114_021beba8(mutexInfo, connection);
    func_ov114_021ba850(connection);
    return 0;
}

// Waits for the thread to be done with a connection that it is receiving the answer of
int func_ov114_021bf288(NHttpConnection *connection) {
    NHttpRoot *root = func_ov114_021bef08();
    NHttpThreadInfo *thread = func_ov114_021bef50(root);
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(root);
    NHttpConnection *found = func_ov114_021bec00(mutexInfo, connection);
    int stage;

    if (found == NULL) {
        return -1;
    }
    stage = func_ov114_021bf3bc((int)connection);
    func_ov114_021b9c1c(thread, TRUE);
    if (stage != -1 && stage != 0) {
        if (func_ov114_021bf488((int)connection) == 15) {
            func_ov114_021bea64(found);
        }
    }
    return 0;
}

// Sends the request of a connection to the thread
int func_ov114_021bf308(int handle) {
    NHttpRoot *root = func_ov114_021bef08();
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(root);
    NHttpConnection *connection = func_ov114_021bec00(mutexInfo, (NHttpConnection *)handle);

    if (connection == NULL) {
        return -1;
    }
    if (connection->request == NULL) {
        return -1;
    }
    connection->id = func_ov114_021bb97c(root, connection->request);
    if (connection->id >= 0) {
        connection->stage = NHTTP_STAGE_QUEUED;
        func_ov114_021befe4(mutexInfo, connection);
    }
    return 0;
}

int func_ov114_021bf374(int handle) {
    NHttpRoot *root = func_ov114_021bef08();
    NHttpConnection *connection = func_ov114_021bec00(func_ov114_021bef58(root), (NHttpConnection *)handle);

    if (connection == NULL) {
        return -1;
    }
    if (connection->id >= 0) {
        func_ov114_021bba14(root, connection->id);
        return 0;
    }
    return -1;
}

int func_ov114_021bf3bc(int handle) {
    NHttpConnection *connection =
        func_ov114_021bec00(func_ov114_021bef58(func_ov114_021bef08()), (NHttpConnection *)handle);

    if (connection == NULL) {
        return -1;
    }
    return connection->stage;
}

int func_ov114_021bf3e4(int handle, char **buffer, int *size) {
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(func_ov114_021bef08());
    NHttpConnection *connection = func_ov114_021bec00(mutexInfo, (NHttpConnection *)handle);
    NHttpResponse *response;

    if (connection != NULL) {
        response = func_ov114_021bebc8(mutexInfo, connection);
        if (response != NULL) {
            *buffer = response->buffer;
            *size = response->bufferSize;
            return response->bufferUsed;
        }
        return -1;
    }
    return -1;
}

void *func_ov114_021bf444(int handle) {
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(func_ov114_021bef08());
    NHttpConnection *connection = func_ov114_021bec00(mutexInfo, (NHttpConnection *)handle);
    NHttpResponse *response;

    if (connection != NULL) {
        response = func_ov114_021bebc8(mutexInfo, connection);
        if (response != NULL) {
            return response->userParam;
        }
        return NULL;
    }
    return NULL;
}

// How the connection ended: 0 if well, an error otherwise, and 15 while it has not ended
int func_ov114_021bf488(int handle) {
    NHttpConnection *connection =
        func_ov114_021bec00(func_ov114_021bef58(func_ov114_021bef08()), (NHttpConnection *)handle);

    if (connection != NULL) {
        return connection->status;
    }
    return -1;
}

// What has been received of the body, and the size that the answer says it has (0 if it does not say)
int func_ov114_021bf4b0(int handle, int *received, int *size) {
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(func_ov114_021bef08());
    NHttpConnection *connection = func_ov114_021bec00(mutexInfo, (NHttpConnection *)handle);
    NHttpResponse *response;

    if (connection != NULL) {
        response = func_ov114_021bebc8(mutexInfo, connection);
        if (response != NULL) {
            *received = response->bodyReceived;
            if (response->contentLength == -1) {
                *size = 0;
            } else {
                *size = response->contentLength;
            }
            return 0;
        }
        return -1;
    }
    return -1;
}
