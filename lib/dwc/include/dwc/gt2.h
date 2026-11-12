// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/gt2.h: its types and constants,
// and the prototypes of Black's named functions.
#ifndef BWPORT_DWC_GT2_H
#define BWPORT_DWC_GT2_H

#include "types.h"


// Notified once with a code (3 for a socket-wide error) and the engine's or a pending connect's userData
typedef void (*Gt2EventCallback)(int code, void *userData);

// Notified with the ip and port of a connect attempt that just finished, and its raw result
typedef void (*Gt2AttemptResultCallback)(u32 ip, u16 port, int reason, BOOL wasRejected, void *userData);

// Notified with the ip and port of a connection that closed, and why
typedef void (*Gt2ClosedCallback)(u32 ip, u16 port, int reason, void *userData);

// Notified with the ip and port of a ping, and its latency in milliseconds
typedef void (*Gt2PingCallback)(u32 ip, u16 port, u32 latencyMs, void *userData);

// Notified with the ip and port of a received datagram, its data and length, whether it is reliable, and userData
typedef void (*Gt2ReceivedCallback)(u32 ip, u16 port, const void *data, int len, BOOL reliable, void *userData);

// A pending connect's own received callback, including the userData supplied to Gt2_Connect
typedef void (*Gt2PendingDataCallback)(u32 ip, u16 port, const void *data, int len, BOOL reliable, void *userData);

// Notified with the ip and port of a datagram with no 16-byte header match, its length, data and userData; its
// result becomes Gt2_CreateSocket's own "was it handled" result
typedef BOOL (*Gt2UnknownMessageCallback)(u32 ip, u16 port, const void *data, int len, void *userData);

// An incoming attempt not claimed by a pending connect, with elapsed milliseconds, init data and userData
typedef void (*Gt2IncomingAttemptCallback)(u32 ip, u16 port, u32 elapsedMilliseconds, const void *data, int len,
                                           void *userData);

// pokebw2's Gt2_IsInitialized
// Whether the engine was initialized, and its bound local port
BOOL func_ov009_02180e48(void);
// pokebw2's Gt2_CreateSocket
// Creates the one engine with its app-wide callbacks; pending connects can supply their own event handlers.
// Buffer sizes are bytes; zero selects 1500 incoming or 1460 outgoing bytes.
// Returns 0 (success), 1 (out of memory), or 3 (the lower socket could not be created)
int func_ov009_02180e58(u16 localPort, int incomingBufferSize, int outgoingBufferSize, Gt2EventCallback onSocketError,
                     Gt2AttemptResultCallback onAttemptResult, Gt2ClosedCallback onClosed, Gt2PingCallback onPing,
                     Gt2ReceivedCallback onReceived, Gt2UnknownMessageCallback onUnknownMessage,
                     Gt2IncomingAttemptCallback onUnhandledAttempt, void *userData);
// pokebw2's Gt2_GetConnectionState
// The state of the connection to an address (its lower-layer handle's own state), written to *state
int func_ov009_02181010(u32 ip, u16 port, int *state);
// pokebw2's Gt2_OpenConnection
// Opens a connection using a 16-byte init message; timeoutMs is milliseconds, or zero for no timeout
int func_ov009_021810cc(u32 ip, u16 port, void *initMessage, u32 timeoutMs);
// pokebw2's Gt2_Send
// Sends a message of size bytes, with a 16-byte header, to the connection of an address
int func_ov009_02181350(u32 ip, u16 port, char *header, const void *message, int size, BOOL reliable);
// pokebw2's Gt2_StopSocket
// Stops the engine's socket without freeing it, and the full teardown
int func_ov009_021814ec(void);
// pokebw2's Gt2_CloseSocket
int func_ov009_02181530(void);
// pokebw2's Gt2_GetLocalPort
u16 func_ov009_0218158c(void);
// pokebw2's Gt2_Connect
// Registers a pending connect; initMessage and header each point to 16 readable bytes with a nonzero first byte.
// Its callbacks are this pending connect's own.
int func_ov009_021815c8(char *initMessage, char *header, Gt2EventCallback onEvent,
                Gt2AttemptResultCallback onAttemptResult, Gt2ClosedCallback onClosed, Gt2PingCallback onPing,
                Gt2PendingDataCallback onReceived, void *userData);
// pokebw2's Gt2_CancelConnect
// Removes the pending connect matching the fixed 16-byte header, whose first byte must be nonzero
int func_ov009_02181748(char *header);
// pokebw2's Gt2_IsIdle
// Whether there are no pending connects left
BOOL func_ov009_02181828(void);
// pokebw2's Gt2_HasNoAppCallbacks
// Whether Gt2_CreateSocket was given none of its app-wide callbacks
BOOL func_ov009_02181874(void);
// pokebw2's Gt2_Close
// Closes an established connection
int func_ov009_021818ec(u32 ip, u16 port);
// pokebw2's Gt2_WStrLen
// A wide-character string's length, unrelated to GT2 itself but public and placed in this file by the original
int func_ov009_0218198c(const u16 *str);

#endif // BWPORT_DWC_GT2_H
