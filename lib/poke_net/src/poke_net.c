// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/poke_net/src/poke_net.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The request engine of the GDS client: it sends a request and receives its answer in a thread of its own, over an SSL
// connection, and the platform layer under it (the allocator, the mutex, the threads and the sockets). The ROM names
// nothing here; the file's name and its functions' are those of the Generation 4 library, or guesses after them. The
// two halves may be two files (poke_net.c and a platform file), which nothing in the ROM tells apart.

#include <string.h>
#include "types.h"
#include "poke_net_internal.h"
#include "dwc/ext.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "gfl/net_devwifi.h"
#include "dwc/dwc.h"
#include "poke_net/poke_net_gds.h"

#define POKE_NET_THREAD_STACK_SIZE 0x800
#define POKE_NET_THREAD_PRIORITY 16
#define POKE_NET_SERVER_URL_LENGTH 0x100

typedef struct {
    POKE_NET_REQUESTCOMMON_AUTH Auth;
    // Whether Auth has its token
    BOOL HasSvlToken;
    int Status;
    char ServerURL[POKE_NET_SERVER_URL_LENGTH];
    u16 ServerPort;
    // The time each step may take, in seconds
    s32 ConnectTimeOut;
    s32 SendTimeOut;
    s32 RecvTimeOut;
    int LastErrorCode;
    s32 SendSize;
    u8 *SendBuffer;
    s32 RecvSize;
    u8 *RecvBuffer;
    // The server's address, in network order
    u32 ServerAddr;
    BOOL AbortFlag;
    int Socket;
    u32 ThreadPriority;
    OSThread Thread;
    OSMutex Mutex;
    // The thread that receives, which blocks in the socket while the request's thread watches the time
    OSThread RecvThread;
    s32 RecvResult;
    s32 RecvOffset;
    BOOL RecvDone;
} POKE_NET_CONTROL;

// The compiler lays these out from the last to the first: the stacks come first in the ROM. A thread is given the top
// of its stack, so each stack's pool word is the address of the object after it
// pokebw2's sPOKE_NET_SslConnection
static POKE_NET_SSL_CONNECTION data_ov125_021da258;
// pokebw2's gPOKE_NET_Control
static POKE_NET_CONTROL data_ov125_021d9e40;
// pokebw2's sPOKE_NET_RecvThreadStack
static u8 data_ov125_021d9640[POKE_NET_THREAD_STACK_SIZE];
// pokebw2's sPOKE_NET_ThreadStack
static u8 data_ov125_021d8e40[POKE_NET_THREAD_STACK_SIZE];

// The seconds since the system timer started, of which a tick is 64 cycles of the system OS_GetTick
#define POKE_NET_TicksToSeconds(tick) ((tick) / (OS_SYSTEM_CLOCK / 64))
#define POKE_NET_GetTime() POKE_NET_TicksToSeconds(OS_GetTick())


void func_ov125_021d7cac(void);
s32 func_ov125_021d891c(s32 reqno);
void *func_ov125_021d7c7c(s32 size);
BOOL func_ov125_021d7d30(void);
void func_ov125_021d8588(void);
BOOL func_ov125_021d7cf4(void);
BOOL func_ov125_021d7cd4(void);
BOOL func_ov125_021d7d88(const char *text);
BOOL func_ov125_021d7e20(const char *host, char *address);
void func_ov125_021d8c14(const char *name, POKE_NET_SSL_CONNECTION *connection);
void func_ov125_021d8c5c(POKE_NET_SSL_CONNECTION *connection);
void func_ov125_021d7cc0(void);
void func_ov125_021d8438(u32 msec);
void func_ov125_021d7c90(void *ptr);
void func_ov125_021d7808(void);
void func_ov125_021d7f60(void);
void func_ov125_021d858c(void);
BOOL func_ov125_021d8444(void);
BOOL func_ov125_021d7e80(void);
BOOL func_ov125_021d7fa0(void);
BOOL func_ov125_021d80e0(void);
BOOL func_ov125_021d824c(void);
BOOL func_ov125_021d7cec(void);
void func_ov125_021d7d20(void);

// pokebw2's POKE_NET_GetStatus



void func_ov125_021d770c(void) {
    func_ov125_021d7808();
    memset(&data_ov125_021d9e40.Auth, 0, sizeof(POKE_NET_REQUESTCOMMON_AUTH));
    data_ov125_021d9e40.HasSvlToken = FALSE;
    data_ov125_021d9e40.ServerURL[0] = '\0';
    data_ov125_021d9e40.ServerPort = 0;
    func_ov125_021d7cec();
    func_ov125_021d7d20();
}

BOOL func_ov125_021d7770(const char *url, u16 port, POKE_NET_REQUESTCOMMON_AUTH *auth) {
    if (auth != NULL) {
        data_ov125_021d9e40.Auth = *auth;
        data_ov125_021d9e40.HasSvlToken = FALSE;
    }
    func_ov125_021d8588();
    if (func_ov125_021d7cf4() == FALSE) {
        return FALSE;
    }
    func_ov125_021d7cd4();
    strcpy(data_ov125_021d9e40.ServerURL, url);
    data_ov125_021d9e40.ServerPort = port;
    data_ov125_021d9e40.SendBuffer = NULL;
    data_ov125_021d9e40.Socket = -1;
    func_ov125_021d7808();
    return TRUE;
}


void func_ov125_021d7808(void) {
    func_ov125_021d7f60();
    if (data_ov125_021d9e40.SendBuffer != NULL) {
        func_ov125_021d7c90(data_ov125_021d9e40.SendBuffer);
        data_ov125_021d9e40.SendBuffer = NULL;
    }
    data_ov125_021d9e40.ConnectTimeOut = 15;
    data_ov125_021d9e40.SendTimeOut = 30;
    data_ov125_021d9e40.RecvTimeOut = 30;
    data_ov125_021d9e40.SendSize = 0;
    data_ov125_021d9e40.RecvSize = 0;
    data_ov125_021d9e40.RecvBuffer = NULL;
    data_ov125_021d9e40.ServerAddr = 0;
    data_ov125_021d9e40.AbortFlag = FALSE;
    data_ov125_021d9e40.Status = POKE_NET_GDS_STATUS_INITIALIZED;
    data_ov125_021d9e40.LastErrorCode = POKE_NET_GDS_LASTERROR_NONE;
    func_ov125_021d858c();
}

void func_ov125_021d7874(void) {
    BOOL end;
    int status;

    for (;;) {
        end = FALSE;
        switch (data_ov125_021d9e40.Status) {
        case POKE_NET_GDS_STATUS_INACTIVE:
        case POKE_NET_GDS_STATUS_INITIALIZED:
        case POKE_NET_GDS_STATUS_ABORTED:
        case POKE_NET_GDS_STATUS_FINISHED:
        case POKE_NET_GDS_STATUS_ERROR:
            end = TRUE;
            break;
        case POKE_NET_GDS_STATUS_REQUEST:
            if (func_ov125_021d8444() == FALSE) {
                status = (data_ov125_021d9e40.LastErrorCode == POKE_NET_GDS_LASTERROR_ABORTED) ? POKE_NET_GDS_STATUS_ABORTED : POKE_NET_GDS_STATUS_ERROR;
                end = TRUE;
            } else {
                status = POKE_NET_GDS_STATUS_NETSETTING;
            }
            func_ov125_021d7cac();
            data_ov125_021d9e40.Status = status;
            break;
        case POKE_NET_GDS_STATUS_NETSETTING:
            if (func_ov125_021d7e80() == FALSE) {
                status = (data_ov125_021d9e40.LastErrorCode == POKE_NET_GDS_LASTERROR_ABORTED) ? POKE_NET_GDS_STATUS_ABORTED : POKE_NET_GDS_STATUS_ERROR;
                end = TRUE;
            } else {
                status = POKE_NET_GDS_STATUS_CONNECTING;
            }
            func_ov125_021d7cac();
            data_ov125_021d9e40.Status = status;
            break;
        case POKE_NET_GDS_STATUS_CONNECTING:
            if (func_ov125_021d7fa0() == FALSE) {
                status = (data_ov125_021d9e40.LastErrorCode == POKE_NET_GDS_LASTERROR_ABORTED) ? POKE_NET_GDS_STATUS_ABORTED : POKE_NET_GDS_STATUS_ERROR;
                end = TRUE;
            } else {
                status = POKE_NET_GDS_STATUS_SENDING;
            }
            func_ov125_021d7cac();
            data_ov125_021d9e40.Status = status;
            break;
        case POKE_NET_GDS_STATUS_SENDING:
            if (func_ov125_021d80e0() == FALSE) {
                status = (data_ov125_021d9e40.LastErrorCode == POKE_NET_GDS_LASTERROR_ABORTED) ? POKE_NET_GDS_STATUS_ABORTED : POKE_NET_GDS_STATUS_ERROR;
                end = TRUE;
            } else {
                status = POKE_NET_GDS_STATUS_RECEIVING;
            }
            if (data_ov125_021d9e40.SendBuffer != NULL) {
                func_ov125_021d7c90(data_ov125_021d9e40.SendBuffer);
                data_ov125_021d9e40.SendBuffer = NULL;
            }
            func_ov125_021d7cac();
            data_ov125_021d9e40.Status = status;
            break;
        case POKE_NET_GDS_STATUS_RECEIVING:
            if (func_ov125_021d824c() == FALSE) {
                if (data_ov125_021d9e40.LastErrorCode == POKE_NET_GDS_LASTERROR_ABORTED) {
                    status = POKE_NET_GDS_STATUS_ABORTED;
                } else {
                    status = POKE_NET_GDS_STATUS_ERROR;
                }
            } else {
                status = POKE_NET_GDS_STATUS_FINISHED;
            }
            end = TRUE;
            func_ov125_021d7cac();
            data_ov125_021d9e40.Status = status;
            break;
        }

        if (end) {
            break;
        }
        if (data_ov125_021d9e40.AbortFlag == 1) {
            data_ov125_021d9e40.Status = POKE_NET_GDS_STATUS_ABORTED;
            data_ov125_021d9e40.LastErrorCode = POKE_NET_GDS_LASTERROR_ABORTED;
            break;
        }
        func_ov125_021d7cc0();
        func_ov125_021d8438(1);
    }
    func_ov125_021d7cc0();
}

BOOL func_ov125_021d7a10(u16 reqno, void *param, void *response, s32 size) {
    s32 reqsize;
    POKE_NET_REQUESTCOMMON *request;

    func_ov125_021d7cac();
    switch (data_ov125_021d9e40.Status) {
    case POKE_NET_GDS_STATUS_INACTIVE:
    case POKE_NET_GDS_STATUS_REQUEST:
    case POKE_NET_GDS_STATUS_NETSETTING:
    case POKE_NET_GDS_STATUS_CONNECTING:
    case POKE_NET_GDS_STATUS_SENDING:
    case POKE_NET_GDS_STATUS_RECEIVING:
        func_ov125_021d7cc0();
        data_ov125_021d9e40.Status = POKE_NET_GDS_STATUS_ERROR;
        data_ov125_021d9e40.LastErrorCode = POKE_NET_GDS_LASTERROR_NOTINITIALIZED;
        return FALSE;
    case POKE_NET_GDS_STATUS_ABORTED:
    case POKE_NET_GDS_STATUS_FINISHED:
    case POKE_NET_GDS_STATUS_ERROR:
        func_ov125_021d7808();
        break;
    }
    func_ov125_021d7cc0();

    data_ov125_021d9e40.RecvSize = func_ov125_021d89ec(reqno);
    if (data_ov125_021d9e40.RecvSize == -1) {
        data_ov125_021d9e40.Status = POKE_NET_GDS_STATUS_ERROR;
        data_ov125_021d9e40.LastErrorCode = POKE_NET_GDS_LASTERROR_ILLEGALREQUEST;
        return FALSE;
    }
    data_ov125_021d9e40.RecvBuffer = response;

    if (size != -1) {
        reqsize = size + POKE_NET_REQUEST_HEADER_SIZE;
    } else {
        reqsize = func_ov125_021d891c(reqno);
        if (reqsize == -1) {
            data_ov125_021d9e40.Status = POKE_NET_GDS_STATUS_ERROR;
            data_ov125_021d9e40.LastErrorCode = POKE_NET_GDS_LASTERROR_ILLEGALREQUEST;
            return FALSE;
        }
    }

    data_ov125_021d9e40.SendBuffer = func_ov125_021d7c7c(reqsize);
    if (data_ov125_021d9e40.SendBuffer == NULL) {
        return FALSE;
    }
    data_ov125_021d9e40.SendSize = reqsize;
    POKE_NET_Memcpy(data_ov125_021d9e40.SendBuffer, &reqsize, POKE_NET_SIZE_LENGTH);
    request = (POKE_NET_REQUESTCOMMON *)(data_ov125_021d9e40.SendBuffer + POKE_NET_SIZE_LENGTH);
    request->ReqCode = reqno;
    request->Auth = data_ov125_021d9e40.Auth;
    request->Option = 0;
    memcpy(request->Param, param, reqsize - POKE_NET_REQUEST_HEADER_SIZE);

    data_ov125_021d9e40.Status = POKE_NET_GDS_STATUS_REQUEST;
    func_ov125_021d7cac();
    if (func_ov125_021d7d30() == FALSE) {
        data_ov125_021d9e40.Status = POKE_NET_GDS_STATUS_ERROR;
        data_ov125_021d9e40.LastErrorCode = POKE_NET_GDS_LASTERROR_CREATETHREAD;
        func_ov125_021d7cc0();
        return FALSE;
    }
    func_ov125_021d7cc0();
    return TRUE;
}



int func_ov125_021d7bcc(void) {
    return data_ov125_021d9e40.Status;
}


void *func_ov125_021d7bdc(void) {
    func_ov125_021d7cac();
    if (func_ov125_021d7bcc() != POKE_NET_GDS_STATUS_FINISHED) {
        func_ov125_021d7cc0();
        return NULL;
    }
    func_ov125_021d7cc0();
    return data_ov125_021d9e40.RecvBuffer + 4;
}

s32 func_ov125_021d7c14(void) {
    func_ov125_021d7cac();
    if (func_ov125_021d7bcc() != POKE_NET_GDS_STATUS_FINISHED) {
        func_ov125_021d7cc0();
        return -1;
    }
    func_ov125_021d7cc0();
    return data_ov125_021d9e40.RecvSize;
}

int func_ov125_021d7c48(void) {
    func_ov125_021d7cac();
    if (func_ov125_021d7bcc() != POKE_NET_GDS_STATUS_ERROR) {
        func_ov125_021d7cc0();
        return 0;
    }
    func_ov125_021d7cc0();
    return data_ov125_021d9e40.LastErrorCode;
}




void *func_ov125_021d7c7c(s32 size) {
    return DWC_Alloc(0, size);
}

void func_ov125_021d7c90(void *ptr) {
    if (ptr == NULL) {
        return;
    }
    DWC_Free(0, ptr, 0);
}


void func_ov125_021d7cac(void) {
    OS_LockMutex(&data_ov125_021d9e40.Mutex);
}

void func_ov125_021d7cc0(void) {
    OS_UnlockMutex(&data_ov125_021d9e40.Mutex);
}




// pokebw2's POKE_NET_CreateMutex
BOOL func_ov125_021d7cd4(void) {
    OS_InitMutex(&data_ov125_021d9e40.Mutex);
    return TRUE;
}

// pokebw2's POKE_NET_ReleaseMutex
BOOL func_ov125_021d7cec(void) {
    return TRUE;
}

// pokebw2's POKE_NET_ExInitialize
BOOL func_ov125_021d7cf4(void) {
    if (!OS_IsTickAvailable()) {
        OS_InitTick();
    }
    data_ov125_021d9e40.ThreadPriority = POKE_NET_THREAD_PRIORITY;
    return TRUE;
}

void func_ov125_021d7d20(void) {
}

void func_ov125_021d7d24(void *arg) {
    func_ov125_021d7874();
}


BOOL func_ov125_021d7d30(void) {
    OS_CreateThread(&data_ov125_021d9e40.Thread, func_ov125_021d7d24, NULL, data_ov125_021d8e40 + POKE_NET_THREAD_STACK_SIZE, POKE_NET_THREAD_STACK_SIZE, data_ov125_021d9e40.ThreadPriority);
    OS_WakeupThreadDirect(&data_ov125_021d9e40.Thread);
    return TRUE;
}




// Whether a text is an address in numbers, four groups of up to three digits
// pokebw2's POKE_NET_IsIPAddress
BOOL func_ov125_021d7d88(const char *text) {
    int digits = 0;
    int dots = 0;

    while (*text != '\0') {
        if (*text >= '0' && *text <= '9') {
            digits++;
            if (digits >= 4) {
                return FALSE;
            }
        } else if (*text == '.') {
            if (digits == 0) {
                return FALSE;
            }
            dots++;
            digits = 0;
            if (dots >= 4) {
                return FALSE;
            }
        } else {
            return FALSE;
        }
        text++;
    }
    if (dots == 3 && digits != 0) {
        return TRUE;
    }
    return FALSE;
}





BOOL func_ov125_021d7e20(const char *host, char *address) {
    SOCHostEnt *entry;

    if (!func_ov125_021d7d88(host)) {
        entry = func_ov009_0215f650(host);
        if (entry != NULL) {
            strcpy(address, func_ov009_0215fa9c(*(SOCInAddr *)entry->addrList[0]));
        } else {
            return FALSE;
        }
    } else {
        strcpy(address, host);
    }
    return TRUE;
}


BOOL func_ov125_021d7e80(void) {
    char address[16];
    u32 addr;
    u32 swapped;

    data_ov125_021d9e40.Socket = func_ov009_0215f428(2, 1, 0);
    if (data_ov125_021d9e40.Socket == -1) {
        data_ov125_021d9e40.LastErrorCode = 3;
        return FALSE;
    }
    if (func_ov125_021d7e20(data_ov125_021d9e40.ServerURL, address) == FALSE) {
        data_ov125_021d9e40.LastErrorCode = 5;
        return FALSE;
    }
    addr = func_ov009_0215c910(address);
    swapped = ((addr >> 8) & 0xff00ff) | ((addr << 8) & 0xff00ff00);
    data_ov125_021d9e40.ServerAddr = (swapped >> 16) | (swapped << 16);
    func_ov125_021d8c14(data_ov125_021d9e40.ServerURL, &data_ov125_021da258);
    if (func_ov009_0215fc80(data_ov125_021d9e40.Socket, &data_ov125_021da258) < 0) {
        data_ov125_021d9e40.LastErrorCode = 15;
        return FALSE;
    }
    return TRUE;
}



void func_ov125_021d7f60(void) {
    if (data_ov125_021d9e40.Socket == -1) {
        return;
    }
    func_ov009_0215f638(data_ov125_021d9e40.Socket, 2);
    func_ov009_0215f644(data_ov125_021d9e40.Socket);
    data_ov125_021d9e40.Socket = -1;
    func_ov125_021d8c5c(&data_ov125_021da258);
}

BOOL func_ov125_021d7fa0(void) {
    SOCSockAddrIn address;
    s32 start;
    int result;

    address.len = 8;
    address.port = (u16)(((data_ov125_021d9e40.ServerPort >> 8) & 0xff) | ((data_ov125_021d9e40.ServerPort << 8) & 0xff00));
    address.family = 2;
    address.addr.addr = data_ov125_021d9e40.ServerAddr;
    start = POKE_NET_GetTime();

    for (;;) {
        result = func_ov009_0215f480(data_ov125_021d9e40.Socket, &address);
        if (result >= 0) {
            break;
        }
        if (result == -26) {
            if (data_ov125_021d9e40.ConnectTimeOut != 0) {
                if ((s32)(POKE_NET_GetTime() - start) >= data_ov125_021d9e40.ConnectTimeOut) {
                    data_ov125_021d9e40.LastErrorCode = 10;
                    return FALSE;
                }
            }
        } else if (result == -30) {
            break;
        } else {
            data_ov125_021d9e40.LastErrorCode = 7;
            return FALSE;
        }

        func_ov125_021d7cac();
        if (data_ov125_021d9e40.AbortFlag == 1) {
            data_ov125_021d9e40.LastErrorCode = 13;
            func_ov125_021d7cc0();
            return FALSE;
        }
        func_ov125_021d7cc0();
        func_ov125_021d8438(1);
    }
    return TRUE;
}

BOOL func_ov125_021d80e0(void) {
    s32 offset;
    s32 start;
    s32 size;
    int result;

    offset = 0;
    start = POKE_NET_GetTime();

    for (;;) {
        size = data_ov125_021d9e40.SendSize - offset;
        if (size > data_ov125_021d9e40.SendSize) {
            size = data_ov125_021d9e40.SendSize;
        }
        result = func_ov009_0215f590(data_ov125_021d9e40.Socket, data_ov125_021d9e40.SendBuffer + offset, size, 0);
        if (result != -6) {
            if (result < 0) {
                data_ov125_021d9e40.LastErrorCode = 11;
                return FALSE;
            }
            offset += result;
            if (offset == data_ov125_021d9e40.SendSize) {
                break;
            }
        }
        if (data_ov125_021d9e40.SendTimeOut != 0) {
            if ((s32)(POKE_NET_GetTime() - start) >= data_ov125_021d9e40.SendTimeOut) {
                data_ov125_021d9e40.LastErrorCode = 11;
                return FALSE;
            }
        }

        func_ov125_021d7cac();
        if (data_ov125_021d9e40.AbortFlag == 1) {
            data_ov125_021d9e40.LastErrorCode = 13;
            func_ov125_021d7cc0();
            return FALSE;
        }
        func_ov125_021d7cc0();
        func_ov125_021d8438(1);
    }
    return TRUE;
}



void func_ov125_021d81ec(void *arg) {
    s32 offset;

    for (;;) {
        offset = data_ov125_021d9e40.RecvOffset;
        func_ov125_021d7cac();
        if (!data_ov125_021d9e40.RecvDone) {
            data_ov125_021d9e40.RecvResult = func_ov009_0215f4dc(data_ov125_021d9e40.Socket, data_ov125_021d9e40.RecvBuffer + offset, data_ov125_021d9e40.RecvSize - offset, 0);
            data_ov125_021d9e40.RecvDone = TRUE;
        }
        func_ov125_021d7cc0();
        func_ov125_021d8438(1);
    }
}

BOOL func_ov125_021d824c(void) {
    u32 offset;
    s32 size;
    s32 start;
    u64 startTick;

    offset = 0;
    size = -1;
    startTick = OS_GetTick();
    OS_CreateThread(&data_ov125_021d9e40.RecvThread, func_ov125_021d81ec, NULL, data_ov125_021d9640 + POKE_NET_THREAD_STACK_SIZE, POKE_NET_THREAD_STACK_SIZE, data_ov125_021d9e40.ThreadPriority);
    data_ov125_021d9e40.RecvResult = -6;
    data_ov125_021d9e40.RecvOffset = 0;
    data_ov125_021d9e40.RecvDone = FALSE;
    OS_WakeupThreadDirect(&data_ov125_021d9e40.RecvThread);
    start = POKE_NET_TicksToSeconds(startTick);

    for (;;) {
        if (data_ov125_021d9e40.RecvTimeOut != 0) {
            if ((s32)(POKE_NET_GetTime() - start) >= data_ov125_021d9e40.RecvTimeOut) {
                data_ov125_021d9e40.LastErrorCode = 12;
                OS_DestroyThread(&data_ov125_021d9e40.RecvThread);
                return FALSE;
            }
        }
        if (!data_ov125_021d9e40.RecvDone) {
            continue;
        }
        if (data_ov125_021d9e40.RecvResult != -6) {
            if (data_ov125_021d9e40.RecvResult <= 0) {
                data_ov125_021d9e40.LastErrorCode = 9;
                OS_DestroyThread(&data_ov125_021d9e40.RecvThread);
                return FALSE;
            }
            offset += data_ov125_021d9e40.RecvResult;
            if (offset >= 4 && size == -1) {
                POKE_NET_Memcpy(&size, data_ov125_021d9e40.RecvBuffer, POKE_NET_SIZE_LENGTH);
            }
            if (offset == size && size != -1) {
                data_ov125_021d9e40.RecvSize = size - 4;
                break;
            }
            data_ov125_021d9e40.RecvOffset = offset;
        }

        func_ov125_021d7cac();
        if (data_ov125_021d9e40.AbortFlag == 1) {
            data_ov125_021d9e40.LastErrorCode = 13;
            func_ov125_021d7cc0();
            OS_DestroyThread(&data_ov125_021d9e40.RecvThread);
            return FALSE;
        }
        func_ov125_021d7cc0();
        data_ov125_021d9e40.RecvDone = FALSE;
        func_ov125_021d8438(1);
    }
    OS_DestroyThread(&data_ov125_021d9e40.RecvThread);
    return TRUE;
}




void func_ov125_021d8438(u32 msec) {
    OS_Sleep(msec);
}





BOOL func_ov125_021d8444(void) {
    DWCSvlResult result;
    int type;
    int code;

    if (data_ov125_021d9e40.HasSvlToken == TRUE) {
        return TRUE;
    }
    if (!DWC_SVLGetTokenAsync("", &result)) {
        data_ov125_021d9e40.LastErrorCode = POKE_NET_GDS_LASTERROR_GETSVL;
        return FALSE;
    }

    for (;;) {
        int state = DWC_SVLProcess();

        if (state == DWC_SVL_STATE_SUCCESS) {
            POKE_NET_Memcpy(data_ov125_021d9e40.Auth.SvlToken, result.svltoken, sizeof(result.svltoken));
            ((POKE_NET_REQUESTCOMMON *)(data_ov125_021d9e40.SendBuffer + POKE_NET_SIZE_LENGTH))->Auth =
                data_ov125_021d9e40.Auth;
            data_ov125_021d9e40.HasSvlToken = TRUE;
            return TRUE;
        } else if (state == DWC_SVL_STATE_ERROR) {
            DWC_GetLastErrorEx(&code, &type);
            data_ov125_021d9e40.LastErrorCode = POKE_NET_GDS_LASTERROR_GETSVL;
            return FALSE;
        } else if (state == DWC_SVL_STATE_CANCELED) {
            data_ov125_021d9e40.LastErrorCode = POKE_NET_GDS_LASTERROR_GETSVL;
            return FALSE;
        }

        func_ov125_021d7cac();
        if (data_ov125_021d9e40.AbortFlag == TRUE) {
            data_ov125_021d9e40.LastErrorCode = POKE_NET_GDS_LASTERROR_ABORTED;
            func_ov125_021d7cc0();
            return FALSE;
        }
        func_ov125_021d7cc0();
        func_ov125_021d8438(1);
    }
    return TRUE;
}



void func_ov125_021d8588(void) {
}

void func_ov125_021d858c(void) {
    func_ov125_021d8b44();
}



#define POKE_NET_Swap16(x) ((u16)((((x) >> 8) & 0xff) | (((x) << 8) & 0xff00)))
#define POKE_NET_Swap8x2(x) ((((x) >> 8) & 0x00ff00ff) | (((x) << 8) & 0xff00ff00))

static inline u32 POKE_NET_Swap32(u32 x) {
    x = POKE_NET_Swap8x2(x);
    return (x >> 16) | (x << 16);
}

