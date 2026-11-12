// Black's code at ov114 0x021bea20-0x021bf04c. Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
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

// The connections, linked by NHttpConnection.next
static NHttpConnection *data_ov114_021cf984;
// The library's state once func_ov114_021bef08 has made it (data_ov114_021cf9c0), or NULL
static NHttpRoot *data_ov114_021cf988;
static NHttpWaitInfo data_ov114_021cf98c;
static NHttpRoot data_ov114_021cf9c0;
#pragma thumb off
NHttpWaitInfo *func_ov114_021bea20(void) {
    NHttpWaitInfo *info = &data_ov114_021cf98c;

    if (!data_ov114_021cf98c.ready) {
        func_ov114_021b9b4c(&info->mutex);
        info->waiters.tail = NULL;
        info->waiters.head = NULL;
        info->ready = TRUE;
    }
    return info;
}

// Sleeps until the connection is not in use any more
void func_ov114_021bea64(NHttpConnection *connection) {
    NHttpWaitInfo *info = func_ov114_021bea20();

    if (connection->running != 0) {
        do {
            OS_SleepThread(&info->waiters);
        } while (connection->running != 0);
    }
}

void func_ov114_021bea98(NHttpConnection *connection) {
    NHttpWaitInfo *info = func_ov114_021bea20();

    connection->running = FALSE;
    OS_WakeupThread(&info->waiters);
}

// Looks for a connection in the list, or puts one in it or takes one out. How it looks for it is the mode: 0 for the
// connection itself, 1 for the one that has a request, 2 for the one that has a response, 3 to put one in and 4 to take
// one out. Returns the connection found, or NULL
NHttpConnection *func_ov114_021beab8(NHttpMutexInfo *mutexInfo, NHttpConnection *key, int mode) {
    NHttpConnection *found = NULL;
    NHttpConnection **link;
    NHttpConnection *node;

    func_ov114_021befcc(mutexInfo);
    if (mode == 3) {
        found = key;
        key->next = data_ov114_021cf984;
        data_ov114_021cf984 = key;
    } else {
        link = &data_ov114_021cf984;
        node = data_ov114_021cf984;
        if (node != NULL) {
            do {
                switch (mode) {
                case 0:
                    if (node == key) {
                        found = node;
                    }
                    break;
                case 1:
                    if (node->request == (NHttpRequest *)key) {
                        found = node;
                    }
                    break;
                case 2:
                    if (node->response == (NHttpResponse *)key) {
                        found = node;
                    }
                    break;
                case 4:
                    if (node == key) {
                        found = *link;
                        *link = found->next;
                    }
                    break;
                }
                if (found != NULL) {
                    break;
                }
                node = *link;
                link = &node->next;
                node = node->next;
            } while (node != NULL);
        }
    }
    func_ov114_021befd8(mutexInfo);
    return found;
}

int func_ov114_021beb88(NHttpMutexInfo *mutexInfo, NHttpConnection *connection) {
    return -(func_ov114_021beab8(mutexInfo, connection, 3) == NULL);
}

int func_ov114_021beba8(NHttpMutexInfo *mutexInfo, NHttpConnection *connection) {
    return -(func_ov114_021beab8(mutexInfo, connection, 4) == NULL);
}

NHttpResponse *func_ov114_021bebc8(NHttpMutexInfo *mutexInfo, NHttpConnection *connection) {
    NHttpConnection *found = func_ov114_021bec00(mutexInfo, connection);

    if (found != NULL) {
        return found->response;
    }
    return NULL;
}

NHttpConnection *func_ov114_021bebe0(NHttpMutexInfo *mutexInfo, NHttpRequest *request) {
    return func_ov114_021beab8(mutexInfo, (NHttpConnection *)request, 1);
}

NHttpConnection *func_ov114_021bebf0(NHttpMutexInfo *mutexInfo, NHttpResponse *response) {
    return func_ov114_021beab8(mutexInfo, (NHttpConnection *)response, 2);
}

NHttpConnection *func_ov114_021bec00(NHttpMutexInfo *mutexInfo, NHttpConnection *connection) {
    return func_ov114_021beab8(mutexInfo, connection, 0);
}

// The request of a connection, or the argument if it is not one
NHttpRequest *func_ov114_021bec10(NHttpMutexInfo *mutexInfo, NHttpConnection *connection) {
    NHttpRequest *request = (NHttpRequest *)connection;
    NHttpConnection *found = func_ov114_021bec00(mutexInfo, connection);

    if (found != NULL) {
        request = found->request;
    }
    return request;
}

NHttpResponse *func_ov114_021bec2c(NHttpMutexInfo *mutexInfo, NHttpConnection *connection) {
    NHttpResponse *response = (NHttpResponse *)connection;
    NHttpConnection *found = func_ov114_021bec00(mutexInfo, connection);

    if (found != NULL) {
        response = found->response;
    }
    return response;
}

int func_ov114_021bec48(void) {
    NHttpConnection *node = data_ov114_021cf984;
    int length = 0;

    if (node != NULL) {
        do {
            length++;
            node = node->next;
        } while (node != NULL);
    }
    return length;
}

// Calls the callback of a connection when the post data asks for more: with the name of the part and how much was
// given, and the buffer the callback sets. Returns what the callback returned, or -1
int func_ov114_021bec74(NHttpMutexInfo *mutexInfo, NHttpConnection *connection, const char *name, int offset) {
    int result = -1;
    NHttpPostEvent event;

    if (func_ov114_021bec00(mutexInfo, connection) != NULL) {
        if (func_ov114_021bebc8(mutexInfo, connection) != NULL && connection->callback != NULL) {
            event.name = name;
            event.buffer = connection->userBuffer;
            event.bufferSize = connection->userBufferSize;
            event.offset = offset;
            result = connection->callback((int)connection, 1, &event);
            func_ov114_021bee4c(mutexInfo, connection, event.buffer, event.bufferSize);
        }
    }
    return result;
}

// Calls the callback of a connection when the buffer of its answer is full, and takes the buffer that it gives
void func_ov114_021bed04(NHttpMutexInfo *mutexInfo, NHttpConnection *connection) {
    NHttpResponse *response;
    NHttpBufferEvent event;

    if (func_ov114_021bec00(mutexInfo, connection) != NULL) {
        response = func_ov114_021bebc8(mutexInfo, connection);
        if (response != NULL && connection->callback != NULL) {
            event.buffer = response->buffer;
            event.bufferSize = response->bufferSize;
            event.used = response->bufferUsed;
            connection->callback((int)connection, 2, &event);
            func_ov114_021bee74(mutexInfo, connection, event.buffer, event.bufferSize, event.used);
        }
    }
}

// The same when the body has been received
void func_ov114_021bed90(NHttpMutexInfo *mutexInfo, NHttpConnection *connection) {
    NHttpResponse *response;
    NHttpBufferEvent event;

    if (func_ov114_021bec00(mutexInfo, connection) != NULL) {
        response = func_ov114_021bebc8(mutexInfo, connection);
        if (response != NULL && connection->callback != NULL) {
            event.buffer = response->buffer;
            event.bufferSize = response->bufferSize;
            event.used = response->bufferUsed;
            connection->callback((int)connection, 3, &event);
            func_ov114_021bee74(mutexInfo, connection, event.buffer, event.bufferSize, event.used);
        }
    }
}

// Calls the callback of a connection when it has ended
void func_ov114_021bee1c(NHttpMutexInfo *mutexInfo, NHttpConnection *connection) {
    if (func_ov114_021bec00(mutexInfo, connection) != NULL && connection->callback != NULL) {
        connection->callback((int)connection, 4, NULL);
    }
}

int func_ov114_021bee4c(NHttpMutexInfo *mutexInfo, NHttpConnection *connection, void *buffer, int size) {
    NHttpConnection *found = func_ov114_021bec00(mutexInfo, connection);

    if (found == NULL) {
        return -1;
    }
    found->userBuffer = buffer;
    found->userBufferSize = size;
    return 0;
}

int func_ov114_021bee74(NHttpMutexInfo *mutexInfo, NHttpConnection *connection, void *buffer, int size,
                                    int used) {
    NHttpConnection *found = func_ov114_021bec00(mutexInfo, connection);
    NHttpResponse *response;

    if (found != NULL) {
        response = func_ov114_021bebc8(mutexInfo, found);
        if (response != NULL) {
            response->buffer = buffer;
            response->bufferSize = size;
            response->bufferUsed = used;
            return 0;
        }
        return -1;
    }
    return -1;
}

void func_ov114_021beec4(NHttpRoot *root) {
    func_ov114_021ba7dc(func_ov114_021bef3c(root));
    func_ov114_021bacec(func_ov114_021bef40(root));
    func_ov114_021bb3c0(func_ov114_021bef48(root));
    func_ov114_021bef98(func_ov114_021bef58(root));
    func_ov114_021bbe94(func_ov114_021bef50(root));
}

NHttpRoot *func_ov114_021bef08(void) {
    NHttpRoot *root = data_ov114_021cf988;

    if (root == NULL) {
        root = &data_ov114_021cf9c0;
        data_ov114_021cf988 = root;
        func_ov114_021beec4(root);
    }
    return root;
}

NHttpState *func_ov114_021bef3c(NHttpRoot *root) {
    return &root->state;
}

NHttpListInfo *func_ov114_021bef40(NHttpRoot *root) {
    return &root->list;
}

NHttpReqInfo *func_ov114_021bef48(NHttpRoot *root) {
    return &root->req;
}

NHttpThreadInfo *func_ov114_021bef50(NHttpRoot *root) {
    return &root->threadInfo;
}

NHttpMutexInfo *func_ov114_021bef58(NHttpRoot *root) {
    return &root->mutexInfo;
}

// Sets how much of the body the connection is to receive, as the chunk in the answer says
void func_ov114_021bef60(NHttpConnection *connection, int length) {
    if (connection != NULL) {
        connection->virtualContentLength = length;
    }
}

void func_ov114_021bef6c(void) {
    if (data_ov114_021cf984 != NULL) {
        func_ov114_021bec48();
    }
    data_ov114_021cf984 = NULL;
}

void func_ov114_021bef98(NHttpMutexInfo *mutexInfo) {
    mutexInfo->ready = FALSE;
}

void func_ov114_021befa4(NHttpMutexInfo *mutexInfo) {
    if (!mutexInfo->ready) {
        func_ov114_021b9b4c(&mutexInfo->mutex);
        mutexInfo->ready = TRUE;
    }
}

void func_ov114_021befc8(void) {
}

void func_ov114_021befcc(NHttpMutexInfo *mutexInfo) {
    func_ov114_021b9b58(&mutexInfo->mutex);
}

void func_ov114_021befd8(NHttpMutexInfo *mutexInfo) {
    OS_UnlockMutex(&mutexInfo->mutex);
}

// Counts the users of a connection, and gives how many there are
int func_ov114_021befe4(NHttpMutexInfo *mutexInfo, NHttpConnection *connection) {
    func_ov114_021befcc(mutexInfo);
    connection->waiters++;
    func_ov114_021befd8(mutexInfo);
    return connection->waiters;
}

int func_ov114_021bf018(NHttpMutexInfo *mutexInfo, NHttpConnection *connection) {
    func_ov114_021befcc(mutexInfo);
    connection->waiters--;
    func_ov114_021befd8(mutexInfo);
    return connection->waiters;
}
