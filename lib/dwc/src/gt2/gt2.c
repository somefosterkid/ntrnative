// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/src/gt2/gt2.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include <string.h>
#include "dwc/gt2.h"
#include "dwc/ext.h"
#include "gs/darray.h"
#include "gs/gt2.h"
#include "gs/nonport.h"
#include "gs/nonport.h"
#include "gs/darray.h"

// Guessed as "gt2.c": Nintendo's "Udp Engine" wrapper around GameSpy's GT2, named only from the error string this
// file's data pool carries, "[Udp Engine] error creating gt2 socket, error code: %d". No existing decomp has
// recovered the real GT2 SDK (gt2.c/gt2callback.c/gt2aux.c), and the lower socket layer this file builds on
// (gt2CreateSocket and the other func_ov011_0217dNNN addresses declared in ext.h) isn't decompiled either, so
// every name and struct field here is a guess read off this file's own behavior, never swan's or the SDK's.

// The one engine this build creates (data_ov009_021ad188); every function below reaches it through func_ov009_02180404
typedef struct GT2Engine {
    GT2Socket socket;                   // the lower layer's socket
    void *connections;                  // GT2EngineConnection array, the established connections, keyed by ip and port
    void *pending;                      // GT2PendingConnect array, keyed by its 16-byte header
    BOOL initialized;                   // whether func_ov009_02180e58 completed successfully
    Gt2AttemptResultCallback onAttemptResult;
    Gt2ClosedCallback onClosed;
    Gt2PingCallback onPing;
    Gt2ReceivedCallback onReceived;
    Gt2IncomingAttemptCallback onUnhandledAttempt;
    Gt2EventCallback onSocketError;
    Gt2UnknownMessageCallback onUnknownMessage;
    void *userData;
    int pendingCount;
    u32 localIp;
    u16 localPort;
} GT2Engine;

// An established connection, keyed by its peer's address
typedef struct GT2EngineConnection {
    u32 ip;
    u16 port;
    GT2Connection handle;               // the lower layer's connection
} GT2EngineConnection;

// A pending connect with a fixed 16-byte init message and header
// Black 2's; twinned code confirms the offsets of header, attempts, onClosed, onPing and userData in Black
typedef struct GT2PendingConnect {
    u8 initMessage[0x10];
    u8 header[0x10];
    // The original uses four-byte entries, but appends a record word or a record pointer on different paths.
    // Completion searches use the connection-address comparator; the inconsistent representation is unresolved.
    void *attempts;
    Gt2ClosedCallback onClosed;
    Gt2PendingDataCallback onReceived;
    Gt2AttemptResultCallback onAttemptResult;
    Gt2PingCallback onPing;
    Gt2EventCallback onEvent;
    void *userData;
} GT2PendingConnect;

GT2Engine *func_ov009_02180404(void);
static void func_ov009_02180410(void *element);
static int func_ov009_02180420(const void *a, const void *b);
static int func_ov009_02180430(const void *a, const void *b);
static int func_ov009_02180448(const void *a, const void *b);
static int func_ov009_02180474(const void *a, const void *b);
static void func_ov009_0218048c(GT2Socket unused);
static void func_ov009_02180518(GT2Connection addr, GT2CloseReason reason);
static void func_ov009_021806a8(GT2Connection addr, GT2Result rawResult, GT2Byte *message, int length);
static void func_ov009_02180924(GT2Connection addr, int latencyMs);
static void func_ov009_02180a64(GT2Connection addr, GT2Byte *data, int len, GT2Bool reliable);
static GT2Bool func_ov009_02180bfc(GT2Socket unused, u32 ip, u16 port, GT2Byte *data, int len);
static void func_ov009_02180c94(GT2Socket unused, GT2Connection handle, u32 ip, u16 port, int elapsedMilliseconds,
                                  GT2Byte *data, int len);

// The one engine this build creates
// pokebw2's sEngine
static GT2Engine data_ov009_021ad188;

// Returns the one engine
// pokebw2's Gt2_GetEngine
GT2Engine *func_ov009_02180404(void) {
    return &data_ov009_021ad188;
}

// The freeFunc of the pending-connect array: frees the attempts it was still racing
// pokebw2's Gt2_FreePendingConnect
static void func_ov009_02180410(void *element) {
    GT2PendingConnect *pending = (GT2PendingConnect *)element;

    ArrayFree(pending->attempts);
}

// Compares the 16-byte init messages at offset 0 of two GT2PendingConnect
// pokebw2's Gt2_CompareInitMessage
static int func_ov009_02180420(const void *a, const void *b) {
    return memcmp(a, b, 0x10);
}

// Compares the 16-byte headers at offset 0x10 of two GT2PendingConnect
// pokebw2's Gt2_ComparePendingHeader
static int func_ov009_02180430(const void *a, const void *b) {
    const GT2PendingConnect *pendingA = a;
    const GT2PendingConnect *pendingB = b;

    return memcmp(pendingA->header, pendingB->header, sizeof(pendingA->header));
}

// Compares the ip and port of two GT2EngineConnection
// pokebw2's Gt2_CompareConnectionAddr
static int func_ov009_02180448(const void *a, const void *b) {
    const GT2EngineConnection *ca = (const GT2EngineConnection *)a;
    const GT2EngineConnection *cb = (const GT2EngineConnection *)b;

    if (ca->ip != cb->ip) {
        return 1;
    }
    if (ca->port != cb->port) {
        return 1;
    }
    return 0;
}

// Compares the lower-layer handle of two GT2EngineConnection
// pokebw2's Gt2_CompareConnectionHandle
static int func_ov009_02180474(const void *a, const void *b) {
    const GT2EngineConnection *ca = (const GT2EngineConnection *)a;
    const GT2EngineConnection *cb = (const GT2EngineConnection *)b;

    return ca->handle != cb->handle;
}

// The lower socket's error callback; notifies the app and every pending connect.
// Its socket argument is unused because this wrapper has one global engine.
// pokebw2's Gt2_OnSocketCreateError
static void func_ov009_0218048c(GT2Socket unused) {
    int i;
    int count;
    GT2Engine *engine = func_ov009_02180404();

    gsDebugFormat(0xc, 0, 0x10, "[Udp Engine] Socket error, passing to app and message handlers\n");
    if (engine->onSocketError != NULL) {
        engine->onSocketError(3, engine->userData);
    }

    count = ArrayLength(engine->pending);
    for (i = 0; i < count; i++) {
        GT2PendingConnect *pending = (GT2PendingConnect *)ArrayNth(engine->pending, i);
        if (pending->onEvent != NULL) {
            pending->onEvent(3, pending->userData);
        }
    }
}

// Notifies that a connection closed, with its mapped reason, then drops it
// pokebw2's Gt2_OnClosed
static void func_ov009_02180518(GT2Connection addr, GT2CloseReason reason) {
    GT2Engine *engine = func_ov009_02180404();
    int i;
    int count;
    int index;
    GT2EngineConnection key;
    char buf[0x18];

    if (reason == 2 || reason == 3) {
        reason = 2;
    } else if (reason == 4) {
        reason = 3;
    }

    gsDebugFormat(0xc, 0, 0x10, "[Udp Engine] Connection closed to %s\n",
                        gt2AddressToString(gt2GetRemoteIP(addr), gt2GetRemotePort(addr), buf));

    count = ArrayLength(engine->pending);
    for (i = 0; i < count; i++) {
        GT2PendingConnect *pending = (GT2PendingConnect *)ArrayNth(engine->pending, i);
        if (pending->onClosed != NULL) {
            gsDebugFormat(0xc, 0, 0x10, "[Udp Engine] Connection closed: passed to message handler\n");
            pending->onClosed(gt2GetRemoteIP(addr), gt2GetRemotePort(addr), reason, pending->userData);
        }
    }

    if (engine->onClosed != NULL) {
        gsDebugFormat(0xc, 0, 0x10, "[Udp Engine] Connection closed: passed to app\n");
        engine->onClosed(gt2GetRemoteIP(addr), gt2GetRemotePort(addr), reason, engine->userData);
    }

    key.handle = addr;
    index = ArraySearch(engine->connections, &key, func_ov009_02180474, 0, FALSE);
    if (index != -1) {
        ArrayDeleteAt(engine->connections, index);
    }
}

// The callback the lower socket calls when one low-level attempt of a func_ov009_021815c8 call finishes; notifies that
// attempt's own handler and the app, and drops the attempt
// pokebw2's Gt2_OnConnectAttemptDone
static void func_ov009_021806a8(GT2Connection addr, GT2Result rawResult, GT2Byte *message, int length) {
    GT2Engine *engine = func_ov009_02180404();
    char buf[0x18];
    GT2EngineConnection connKey;
    GT2EngineConnection attemptKey;
    int reason;
    int index;
    int i;
    int count;

    (void)message;
    (void)length;
    switch (rawResult) {
    case 7:
        reason = 7;
        break;
    case 2:
        reason = 2;
        break;
    case 6:
        reason = 6;
        break;
    case 0:
        reason = 0;
        break;
    default:
        reason = 0xd;
        break;
    }

    if (rawResult == 2) {
        connKey.ip = gt2GetRemoteIP(addr);
        connKey.port = gt2GetRemotePort(addr);
        index = ArraySearch(engine->connections, &connKey, func_ov009_02180448, 0, FALSE);
        if (index != -1) {
            gsDebugFormat(0xc, 0, 0x10, "[Udp Engine] Connect rejected by %s\n",
                          gt2AddressToString(gt2GetRemoteIP(addr), gt2GetRemotePort(addr), buf));
            ArrayDeleteAt(engine->connections, index);
        }
    }

    count = ArrayLength(engine->pending);
    for (i = 0; i < count; i++) {
        GT2PendingConnect *pending = (GT2PendingConnect *)ArrayNth(engine->pending, i);

        attemptKey.ip = gt2GetRemoteIP(addr);
        attemptKey.port = gt2GetRemotePort(addr);
        index = ArraySearch(pending->attempts, &attemptKey, func_ov009_02180448, 0, FALSE);
        if (index != -1) {
            if (pending->onAttemptResult != NULL) {
                BOOL rejected = FALSE;
                gsDebugFormat(0xc, 0, 0x10, "[Udp Engine] Passing connect result to message handler\n");
                if (rawResult == 2) {
                    rejected = TRUE;
                }
                pending->onAttemptResult(gt2GetRemoteIP(addr), gt2GetRemotePort(addr), reason,
                                         rejected, pending->userData);
            }
            ArrayDeleteAt(pending->attempts, index);
            return;
        }
    }

    if (engine->pendingCount > 0) {
        if (engine->onAttemptResult != NULL) {
            BOOL rejected = FALSE;
            gsDebugFormat(0xc, 0, 0x10, "[Udp Engine] Passing connect result to app\n");
            if (rawResult == 2) {
                rejected = TRUE;
            }
            engine->onAttemptResult(gt2GetRemoteIP(addr), gt2GetRemotePort(addr), reason, rejected,
                                    engine->userData);
        }
        engine->pendingCount--;
    }
}

// Reports ping latency to the first pending connect with a ping callback, otherwise to the app
// pokebw2's Gt2_OnPing
static void func_ov009_02180924(GT2Connection addr, int latencyMs) {
    GT2Engine *engine = func_ov009_02180404();
    char buf[0x18];
    int i;
    int count;

    gsDebugFormat(0xc, 0, 0x10, "[Udp Engine] Received ping from %s\n",
                        gt2AddressToString(gt2GetRemoteIP(addr), gt2GetRemotePort(addr), buf));

    count = ArrayLength(engine->pending);
    for (i = 0; i < count; i++) {
        GT2PendingConnect *pending = (GT2PendingConnect *)ArrayNth(engine->pending, i);
        if (pending->onPing != NULL) {
            gsDebugFormat(0xc, 0, 0x10, "[Udp Engine] Passed to message handler\n");
            pending->onPing(gt2GetRemoteIP(addr), gt2GetRemotePort(addr), latencyMs, pending->userData);
            return;
        }
    }

    if (engine->onPing != NULL) {
        gsDebugFormat(0xc, 0, 0x10, "[Udp Engine] Passed to app\n");
        engine->onPing(gt2GetRemoteIP(addr), gt2GetRemotePort(addr), latencyMs, engine->userData);
    }
}

// The callback for a received datagram; if its first 16 bytes match a pending connect's header, notifies
// that connect's own handler, otherwise the app
// pokebw2's Gt2_OnReceivedData
static void func_ov009_02180a64(GT2Connection addr, GT2Byte *data, int len, GT2Bool reliable) {
    GT2Engine *engine = func_ov009_02180404();
    GT2PendingConnect key;
    char buf[0x18];
    int index;

    gsDebugFormat(0xc, 0, 0x10, "[Udp Engine] Received data from %s\n",
                  gt2AddressToString(gt2GetRemoteIP(addr), gt2GetRemotePort(addr), buf));

    if (len >= 0x10) {
        memcpy(key.header, data, sizeof(key.header));
        index = ArraySearch(engine->pending, &key, func_ov009_02180430, 0, FALSE);
        if (index != -1) {
            GT2PendingConnect *pending = (GT2PendingConnect *)ArrayNth(engine->pending, index);
            if (pending->onReceived != NULL) {
                gsDebugFormat(0xc, 0, 0x10, "[Udp Engine] Passed to message handler\n");
                pending->onReceived(gt2GetRemoteIP(addr), gt2GetRemotePort(addr), data + 0x10,
                                    len - 0x10, reliable, pending->userData);
                return;
            }
        }
    }

    if (engine->onReceived != NULL) {
        gsDebugFormat(0xc, 0, 0x10, "[Udp Engine] Passed to app\n");
        engine->onReceived(gt2GetRemoteIP(addr), gt2GetRemotePort(addr), data, len, reliable, engine->userData);
    }
}

// Handles a datagram offered by the lower socket's unknown-message path; returns whether the app handled it
// pokebw2's Gt2_OnUnknownMessage
static GT2Bool func_ov009_02180bfc(GT2Socket unused, u32 ip, u16 port, GT2Byte *data, int len) {
    GT2Engine *engine = func_ov009_02180404();
    char buf[0x18];
    BOOL handled;
    const char *address;

    if (engine->onUnknownMessage != NULL) {
        address = gt2AddressToString(ip, port, buf);
        handled = FALSE;
        gsDebugFormat(0xc, 0, 0x10, "[Udp Engine] Unknown message from %s, passing to app\n", address);
        if (engine->onUnknownMessage(ip, port, data, len, engine->userData) != 0) {
            handled = TRUE;
        }
        return handled;
    }
    return FALSE;
}

// The lower socket's incoming-attempt callback; auto-accepts when the first 16 data bytes match a pending
// connect's init message, otherwise asks the app whether to accept it
static void func_ov009_02180c94(GT2Socket unused, GT2Connection handle, u32 ip, u16 port, int elapsedMilliseconds,
                                  GT2Byte *data, int len) {
    GT2Engine *engine = func_ov009_02180404();
    GT2PendingConnect key;
    GT2EngineConnection newConnection;
    char buf[0x18];
    GT2ConnectionCallbacks table;
    int index;

    gsDebugFormat(0xc, 0, 0x10, "[Udp Engine] Connection attempt from %s\n", gt2AddressToString(ip, port, buf));

    if (len >= 0x10) {
        memcpy(key.initMessage, data, sizeof(key.initMessage));

        newConnection.ip = ip;
        newConnection.port = port;
        newConnection.handle = handle;
        ArrayAppend(engine->connections, &newConnection);
        index = ArraySearch(engine->pending, &key, func_ov009_02180420, 0, FALSE);
        if (index != -1) {
            table.closed = func_ov009_02180518;
            table.connected = func_ov009_021806a8;
            table.ping = func_ov009_02180924;
            table.received = func_ov009_02180a64;
            gt2Accept(newConnection.handle, &table);

            gsDebugFormat(0xc, 0, 0x10, "[Udp Engine] Connection attempt auto-accepted for message handler\n");
            return;
        }
    }

    if (engine->onUnhandledAttempt != NULL) {
        gsDebugFormat(0xc, 0, 0x10, "[Udp Engine] Connection attempt from %s, asking app to accept/reject\n",
                      gt2AddressToString(ip, port, buf));
        engine->onUnhandledAttempt(ip, port, elapsedMilliseconds, data, len, engine->userData);
        return;
    }

    gt2Reject(handle, NULL, 0);
    ArrayRemoveAt(engine->connections, ArrayLength(engine->connections) - 1);
}

// Whether the engine was initialized
// pokebw2's Gt2_IsInitialized
BOOL func_ov009_02180e48(void) {
    return func_ov009_02180404()->initialized;
}

// Creates the one engine, binding its socket to localPort
int func_ov009_02180e58(u16 localPort, int incomingBufferSize, int outgoingBufferSize, Gt2EventCallback onSocketError,
                     Gt2AttemptResultCallback onAttemptResult, Gt2ClosedCallback onClosed, Gt2PingCallback onPing,
                     Gt2ReceivedCallback onReceived, Gt2UnknownMessageCallback onUnknownMessage,
                     Gt2IncomingAttemptCallback onUnhandledAttempt, void *userData) {
    GT2Engine *engine = func_ov009_02180404();
    char addrBuf[0x18];
    int error;

    if (incomingBufferSize == 0) {
        incomingBufferSize = 0x5dc;
    }
    if (outgoingBufferSize == 0) {
        outgoingBufferSize = 0x5b4;
    }

    engine->onSocketError = onSocketError;
    engine->onUnknownMessage = onUnknownMessage;
    engine->onAttemptResult = onAttemptResult;
    engine->onClosed = onClosed;
    engine->onPing = onPing;
    engine->onReceived = onReceived;
    engine->onUnhandledAttempt = onUnhandledAttempt;

    gt2AddressToString(0, localPort, addrBuf);
    error =
        gt2CreateSocket(&engine->socket, addrBuf, outgoingBufferSize, incomingBufferSize, func_ov009_0218048c);
    if (error != 0) {
        gsDebugFormat(0xc, 0, 2, "[Udp Engine] error creating gt2 socket, error code: %d\n", error);
        return 3;
    }

    engine->connections = ArrayNew(0xc, 1, NULL);
    if (engine->connections == NULL) {
        gsDebugFormat(0xc, 2, 1, "[Udp Engine] No more memory!!!\n");
        return 1;
    }

    engine->pending = ArrayNew(0x3c, 1, func_ov009_02180410);
    if (engine->pending == NULL) {
        gsDebugFormat(0xc, 2, 1, "[Udp Engine] No more memory!!!\n");
        return 1;
    }

    gt2SetUnrecognizedMessageCallback(engine->socket, func_ov009_02180bfc);
    gt2Listen(engine->socket, func_ov009_02180c94);
    engine->localIp = gt2GetLocalIP(engine->socket);
    engine->localPort = (u16)gt2GetLocalPort(engine->socket);
    engine->pendingCount = 0;
    engine->initialized = TRUE;
    if (userData != NULL) {
        engine->userData = userData;
    } else {
        engine->userData = NULL;
    }
    return 0;
}

// The lower-layer state of the connection to an address, written to *state
// pokebw2's Gt2_GetConnectionState
int func_ov009_02181010(u32 ip, u16 port, int *state) {
    GT2Engine *engine = func_ov009_02180404();
    GT2EngineConnection key;
    GT2EngineConnection *conn;
    int index;

    if (engine->initialized == 0) {
        gsDebugFormat(0xc, 3, 0xf, "[Udp Engine] Engine not initialized\n");
        *state = 3;
        return 0xb;
    }

    key.ip = ip;
    key.port = port;
    index = ArraySearch(engine->connections, &key, func_ov009_02180448, 0, FALSE);
    if (index == -1) {
        *state = 3;
        return 0;
    }
    conn = (GT2EngineConnection *)ArrayNth(engine->connections, index);
    *state = gt2GetConnectionState(conn->handle);
    return 0;
}

// Reuses a known connection or starts one through the lower layer, then associates it with a pending init message
// pokebw2's Gt2_OpenConnection
int func_ov009_021810cc(u32 ip, u16 port, void *initMessage, u32 timeoutMs) {
    GT2Engine *engine = func_ov009_02180404();
    char buf[0x18];
    GT2EngineConnection key;
    GT2PendingConnect pendingKey;
    GT2ConnectionCallbacks table;
    GT2EngineConnection *conn;
    GT2EngineConnection *last;
    int index;
    int state;

    if (engine->initialized == 0) {
        gsDebugFormat(0xc, 0, 0xf, "[Udp Engine] Engine not initialized\n");
        return 3;
    }
    if (ip == 0 || port == 0) {
        gsDebugFormat(0xc, 0, 0xf, "[Udp Engine] Invalid parameter(s), check ip, port");
        return 0xa;
    }

    key.ip = ip;
    key.port = port;
    index = ArraySearch(engine->connections, &key, func_ov009_02180448, 0, FALSE);
    if (index != -1) {
        conn = (GT2EngineConnection *)ArrayNth(engine->connections, index);
        state = gt2GetConnectionState(conn->handle);
        if (state == 1) {
            gsDebugFormat(0xc, 0, 0xf, "[Udp Engine] Engine is already talking to remote address\n");
            return 5;
        }
        if (state != 0) {
            goto error;
        }

        memcpy(pendingKey.initMessage, initMessage, sizeof(pendingKey.initMessage));
        index = ArraySearch(engine->pending, &pendingKey, func_ov009_02180420, 0, FALSE);
        if (index != -1) {
            ArrayAppend(((GT2PendingConnect *)ArrayNth(engine->pending, index))->attempts, conn);
        }
        goto error;
    }

    gt2AddressToString(ip, port, buf);
    table.closed = func_ov009_02180518;
    table.connected = func_ov009_021806a8;
    table.ping = func_ov009_02180924;
    table.received = func_ov009_02180a64;
    gt2Connect(engine->socket, &key.handle, buf, initMessage, 0x10, timeoutMs, &table, GT2False);

    ArrayAppend(engine->connections, &key);
    memcpy(pendingKey.initMessage, initMessage, sizeof(pendingKey.initMessage));
    index = ArraySearch(engine->pending, &pendingKey, func_ov009_02180420, 0, FALSE);
    if (index == -1) {
        engine->pendingCount++;
        goto error;
    }
    last = (GT2EngineConnection *)ArrayNth(engine->connections, ArrayLength(engine->connections) - 1);
    ArrayAppend(((GT2PendingConnect *)ArrayNth(engine->pending, index))->attempts, &last);
    error:
    return 0;
}

// Sends a message to the connection of an address, with a 16-byte header prepended
// pokebw2's Gt2_Send
int func_ov009_02181350(u32 ip, u16 port, char *header, const void *message, int size, BOOL reliable) {
    GT2Engine *engine = func_ov009_02180404();
    GT2EngineConnection key;
    char buf[0x18];
    GT2EngineConnection *conn;
    void *packet;
    int index;
    int total;
    GT2Result result;

    if (engine->initialized == 0) {
        gsDebugFormat(0xc, 0, 0xf, "[Udp Engine] Engine not initialized\n");
        return 3;
    }

    if (header[0] != 0) {
        total = size + 0x10;
    } else {
        total = size;
    }

    key.ip = ip;
    key.port = port;
    index = ArraySearch(engine->connections, &key, func_ov009_02180448, 0, FALSE);
    if (index == -1) {
        gt2AddressToString(ip, port, buf);
        gsDebugFormat(0xc, 0, 2, "[Udp Engine] address not found for sending message\n", buf);
        return 4;
    }

    conn = (GT2EngineConnection *)ArrayNth(engine->connections, index);
    if (total > gt2GetOutgoingBufferSize(conn->handle) && reliable != 0) {
        gsDebugFormat(0xc, 0, 2, "[Udp Engine] Message Size too large, dropping message\n");
        return 0xc;
    }

    packet = gsimalloc(total);
    memcpy(packet, header, 0x10);
    memcpy((u8 *)packet + 0x10, message, size);
    result = gt2Send(conn->handle, packet, total, reliable);
    gsifree(packet);
    return (result != 0) ? 8 : 0;
}

// Stops the engine's socket without freeing it
// pokebw2's Gt2_StopSocket
int func_ov009_021814ec(void) {
    GT2Engine *engine = func_ov009_02180404();

    if (engine->initialized == 0) {
        gsDebugFormat(0xc, 0, 0xf, "[Udp Engine] Engine not initialized\n");
        return 3;
    }
    gt2Think(engine->socket);
    return 0;
}

// Tears the engine down entirely
// pokebw2's Gt2_CloseSocket
int func_ov009_02181530(void) {
    GT2Engine *engine = func_ov009_02180404();

    if (engine->initialized == 0) {
        gsDebugFormat(0xc, 0, 0xf, "[Udp Engine] Engine not initialized\n");
        return 3;
    }
    gt2CloseSocket(engine->socket);
    ArrayFree(engine->pending);
    ArrayFree(engine->connections);
    engine->initialized = FALSE;
    return 0;
}

// The engine's own bound local port
// pokebw2's Gt2_GetLocalPort
u16 func_ov009_0218158c(void) {
    GT2Engine *engine = func_ov009_02180404();

    if (engine->initialized == 0) {
        gsDebugFormat(0xc, 0, 1, "[Udp Engine] Engine not initialized\n");
        return 0;
    }
    return engine->localPort;
}

// Registers a pending connect; initMessage and header each point to 16 readable bytes with a nonzero first byte
int func_ov009_021815c8(char *initMessage, char *header, Gt2EventCallback onEvent,
                        Gt2AttemptResultCallback onAttemptResult, Gt2ClosedCallback onClosed,
                        Gt2PingCallback onPing, Gt2PendingDataCallback onReceived, void *userData) {
    GT2Engine *engine = func_ov009_02180404();
    GT2PendingConnect pending;
    void *attempts;

    if (engine->initialized == 0) {
        gsDebugFormat(0xc, 0, 0xf, "[Udp Engine] Engine not initialized\n");
        return 3;
    }
    if (initMessage[0] == 0) {
        gsDebugFormat(0xc, 0, 0xf, "[Udp Engine] Invalid init message\n");
        return 0xa;
    }
    if (header[0] == 0) {
        gsDebugFormat(0xc, 0, 0xf, "[Udp Engine] Invalid header\n");
        return 0xa;
    }

    pending.onClosed = onClosed;
    pending.onAttemptResult = onAttemptResult;
    pending.onPing = onPing;
    pending.onReceived = onReceived;
    pending.onEvent = onEvent;
    memcpy(pending.initMessage, initMessage, sizeof(pending.initMessage));
    memcpy(pending.header, header, sizeof(pending.header));

    attempts = ArrayNew(4, 1, NULL);
    pending.attempts = attempts;
    if (attempts == NULL) {
        gsDebugFormat(0xc, 2, 1, "[Udp Engine] No more memory!!!\n");
        return 1;
    }

    pending.userData = userData;
    ArrayAppend(engine->pending, &pending);
    return 0;
}

// Removes the pending connect with a matching 16-byte header
// pokebw2's Gt2_CancelConnect
int func_ov009_02181748(char *header) {
    GT2Engine *engine = func_ov009_02180404();
    GT2PendingConnect key;
    int index;

    if (engine->initialized == 0) {
        gsDebugFormat(0xc, 0, 0xf, "[Udp Engine] Engine not initialized\n");
        return 3;
    }
    if (header == NULL || header[0] == 0) {
        gsDebugFormat(0xc, 0, 0xf, "[Udp Engine] invalid or empty header\n");
        return 0xa;
    }

    memcpy(key.header, header, sizeof(key.header));

    index = ArraySearch(engine->pending, &key, func_ov009_02180430, 0, FALSE);
    if (index != -1) {
        ArrayDeleteAt(engine->pending, index);
    }
    return 0;
}

// Whether there are no pending connects left
// pokebw2's Gt2_IsIdle
BOOL func_ov009_02181828(void) {
    GT2Engine *engine = func_ov009_02180404();

    if (engine->initialized == 0) {
        gsDebugFormat(0xc, 0, 0xf, "[Udp Engine] Engine not initialized\n");
        return TRUE;
    }
    return ArrayLength(engine->pending) == 0;
}

// Whether func_ov009_02180e58 was given none of its app-wide callbacks
// pokebw2's Gt2_HasNoAppCallbacks
BOOL func_ov009_02181874(void) {
    GT2Engine *engine = func_ov009_02180404();

    if (engine->initialized == 0) {
        gsDebugFormat(0xc, 0, 0xf, "[Udp Engine] Engine not initialized\n");
        return TRUE;
    }
    if (engine->onClosed != NULL || engine->onUnhandledAttempt != NULL || engine->onAttemptResult != NULL ||
        engine->onSocketError != NULL || engine->onPing != NULL || engine->onReceived != NULL ||
        engine->onUnknownMessage != NULL) {
        return FALSE;
    }
    return TRUE;
}

// Closes an established connection
// pokebw2's Gt2_Close
int func_ov009_021818ec(u32 ip, u16 port) {
    GT2Engine *engine = func_ov009_02180404();
    GT2EngineConnection key;
    GT2EngineConnection *conn;
    int index;

    if (engine->initialized == 0) {
        gsDebugFormat(0xc, 0, 0xf, "[Udp Engine] Engine not initialized\n");
        return 0;
    }

    key.ip = ip;
    key.port = port;
    index = ArraySearch(engine->connections, &key, func_ov009_02180448, 0, FALSE);
    if (index != -1) {
        conn = (GT2EngineConnection *)ArrayNth(engine->connections, index);
        return gt2GetOutgoingBufferFreeSpace(conn->handle);
    }
    return 0;
}

// A wide-character string's length
// pokebw2's Gt2_WStrLen
int func_ov009_0218198c(const u16 *str) {
    const u16 *p = str;

    while (*p++ != 0) {
    }
    return p - str - 1;
}
