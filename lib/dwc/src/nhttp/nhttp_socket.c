// Black's code at ov114 0x021b9c6c-0x021ba100. Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
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

static u32 data_ov114_021cf964[8];
#pragma thumb off
u32 func_ov114_021b9c6c(u32 flags) {
    if (flags & 0x8000) {
        flags &= ~0x8000;
    }
    return flags;
}

u32 func_ov114_021b9c78(u32 flags) {
    return 0;
}

void func_ov114_021b9c80(void) {
    OS_GetLowEntropyData(data_ov114_021cf964);
    func_ov009_02162248(data_ov114_021cf964, sizeof(data_ov114_021cf964));
}

// Whether the socket still has its connection: 0 if reading would wait, -1 if not
int func_ov114_021b9ca4(int sock) {
    int result = 0;
    u8 byte;

    if (sock == -1) {
        result = -1;
    } else if (func_ov009_0215f4dc(sock, &byte, 1, 4) != -6) {
        result = -1;
    }
    return result;
}

int func_ov114_021b9ce4(NHttpRequest *request) {
    return func_ov009_0215f428(2, 1, 0);
}

// Closes the socket, trying again while the library says it is busy, and frees the secure connection's settings
int func_ov114_021b9cfc(NHttpMutexInfo *mutexInfo, NHttpRequest *request, int sock) {
    int result = func_ov009_0215f644(sock);

    if (result == 0 || result == -26) {
        int waited = 0;

        while ((result = func_ov009_0215f644(sock)) == -26 && waited <= 10000) {
            OS_Sleep(500);
            func_ov009_0215ebb4();
            waited += 500;
        }
    }
    if (result != -26 && request != NULL && request->ssl != NULL) {
        func_ov114_021ba850(request->ssl);
        request->ssl = NULL;
    }
    return result;
}

int func_ov114_021b9d8c(NHttpState *state, NHttpMutexInfo *mutexInfo, NHttpRequest *request, int sock, u32 addr, int port) {
    SOCSockAddrIn address;
    u16 hostPort;

    address.len = sizeof(address);
    address.family = 2;
    hostPort = port;
    address.port = ((hostPort >> 8) & 0xff) | ((hostPort << 8) & 0xff00);
    address.addr.addr = addr;
    if (request->https != 0 && request->useProxy == 0) {
        int result = func_ov114_021b9e3c(state, mutexInfo, request, sock);

        if (result < 0) {
            return result;
        }
    }
    if (func_ov009_0215f480(sock, &address) < 0) {
        if (request->canceled != 0) {
            return NHTTP_SOCKET_CANCELED;
        }
        return NHTTP_SOCKET_ERROR;
    }
    return 0;
}

// Makes the socket's connection secure, with the request's root certificates and its host to check the server's
// against
int func_ov114_021b9e3c(NHttpState *state, NHttpMutexInfo *mutexInfo, NHttpRequest *request, int sock) {
    NHttpSslInfo *ssl = request->ssl;

    if (ssl == NULL) {
        ssl = func_ov114_021ba820(sizeof(NHttpSslInfo), 4);
        request->ssl = ssl;
        func_ov114_021ba13c(ssl, sizeof(NHttpSslInfo));
    }
    ssl->rootCerts = request->rootCerts;
    ssl->rootCertCount = request->rootCertCount;
    ssl->hostName = request->host;
    if (request->skipVerify != 0) {
        ssl->filter = func_ov114_021b9c78;
    } else {
        ssl->filter = func_ov114_021b9c6c;
    }
    if (func_ov009_0215fc80(sock, ssl) < 0) {
        return NHTTP_SOCKET_ERROR;
    }
    func_ov114_021b9c80();
    return 0;
}

// Reads from the socket into the connection's buffer, and gives the caller what it asked for out of it
int func_ov114_021b9ecc(NHttpConnection *connection, int sock, void *buffer, int size, int flags) {
    u8 *recvBuffer = connection->recvBuffer;
    int copied = 0;

    if (size > 0) {
        u32 count;

        if (connection->recvSize == 0) {
            int result = func_ov009_0215f4dc(sock, recvBuffer, NHTTP_RECV_BUFFER_SIZE, flags);

            if (result <= 0) {
                return result;
            }
            connection->recvSize = result;
            connection->recvPos = 0;
        }
        count = connection->recvSize;
        if (count != 0) {
            if (size <= count) {
                count = size;
            }
            func_ov114_021ba100(buffer, recvBuffer + connection->recvPos, count);
            connection->recvSize -= count;
            if (connection->recvSize == 0) {
                func_ov114_021ba13c(recvBuffer, NHTTP_RECV_BUFFER_SIZE);
                connection->recvPos = 0;
            } else {
                connection->recvPos += count;
            }
            copied = count;
        }
    }
    return copied;
}

int func_ov114_021b9f94(NHttpMutexInfo *mutexInfo, NHttpRequest *request, int sock, void *buffer, int size, int flags) {
    NHttpConnection *connection = func_ov114_021bebe0(mutexInfo, request);
    int result;

    if (connection != NULL) {
        result = func_ov114_021b9ecc(connection, sock, buffer, size, flags);
    } else {
        return NHTTP_SOCKET_ERROR;
    }
    if (result < 0) {
        if (request->canceled != 0) {
            return NHTTP_SOCKET_CANCELED;
        }
        if (result == -56) {
            return 0;
        }
        return NHTTP_SOCKET_ERROR;
    }
    return result;
}

int func_ov114_021ba014(int sock, const void *buffer, int size, int flags) {
    return func_ov009_0215f590(sock, buffer, size, flags);
}

int func_ov114_021ba020(NHttpRequest *request, int sock, const void *buffer, int size, int flags) {
    int result = func_ov114_021ba014(sock, buffer, size, flags);

    if (result < 0) {
        if (request->canceled != 0) {
            return NHTTP_SOCKET_CANCELED;
        }
        if (result == -56) {
            return 0;
        }
        return NHTTP_SOCKET_ERROR;
    }
    return result;
}

// Shuts the socket down, which ends a read or a write that waits on it
void func_ov114_021ba068(NHttpMutexInfo *mutexInfo, NHttpRequest *request, int sock) {
    func_ov114_021befcc(mutexInfo);
    if (sock >= 0) {
        func_ov009_0215f638(sock, 2);
    }
    func_ov114_021befd8(mutexInfo);
}

// Looks a host up, as an address or as a name, and returns its address (0 if there is none)
u32 func_ov114_021ba098(NHttpRequest *request, const char *host) {
    u32 address = 0;
    u32 parsed;

    if (func_ov009_0215fad0(host, &parsed) != 0) {
        address = parsed;
    } else {
        SOCHostEnt *entry = func_ov009_0215f650(host);

        if (entry != NULL && entry->addrLength > 0) {
            func_ov114_021ba100(&address, entry->addrList[0], 4);
        }
    }
    return address;
}
