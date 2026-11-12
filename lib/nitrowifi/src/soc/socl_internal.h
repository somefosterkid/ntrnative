// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2) at 9a3236bc8, lib/nitrowifi/src/soc/socl_internal.h.
#ifndef BWPORT_NITROWIFI_SOCL_INTERNAL_H
#define BWPORT_NITROWIFI_SOCL_INTERNAL_H

#include "nitrowifi/soc.h"

// SOCL's state, shared by its files

// socl_startup.c
// socl_startup.c's globals: its functions reach them from one base (-ipa file), the other files by the address of each
// pokebw2's SOCLiRequestedIP
extern CPSInAddr data_ov009_021aa444;     // The address that DHCP asks for
// pokebw2's SOCLiDhcpState
extern u32 data_ov009_021aa448;             // SOCL_DHCP_*
// pokebw2's SOCLiCPSConfig
extern CPSConfig data_ov009_021aa450;

// pokebw2's SOCLi_Alloc and SOCLi_Free: the memory functions of the config
static inline void *SOCLi_Alloc(u32 size) {
    return data_ov009_021a1cd8.config->alloc(size);
}

static inline void SOCLi_Free(void *ptr) {
    data_ov009_021a1cd8.config->free(ptr);
}

// A command pipe: the queue of the packets that its thread runs (the socket's fields at 0xa0 and 0xa4 point to its
// receive and send pipes, and a socket without a receive pipe uses the send pipe for control). Its messages and the
// thread's stack follow the pipe in the socket's memory.
typedef struct SOCLCommandPipe {
    OSMessageQueue queue;
    OSThread thread;
    OSMutex mutex;
} SOCLCommandPipe;

// A datagram that func_ov009_0215df08 queued, followed by its data
typedef struct SOCLUdpPacket {
    struct SOCLUdpPacket *next;
    u16 len;
    u16 port;
    CPSInAddr ip;
    u8 data[];
} SOCLUdpPacket;

// The receive pipe, 0x114 bytes
typedef struct SOCLRecvPipe {
    SOCLCommandPipe base;
    volatile int cps_read_len; // What the reads have taken from CPS's receive buffer and not yet consumed
    u16 rcvbuf_consume_min; // From SOCLSocketParam: how much to take before func_ov009_0215b064
    s8 peek;                // 1 while a peeking read runs: it takes the data without moving the position
    u8 unk_ff;
    SOCLUdpPacket *volatile udp_tail; // The datagrams that came in, oldest first
    SOCLUdpPacket *volatile udp_head;
    volatile u16 udp_queued; // The bytes of datagram data they hold
    u16 udp_queued_max;     // From SOCLSocketParam's rcvpipe_unk_e
    OSThreadQueue udp_wait; // Where a blocking UDP read sleeps
} SOCLRecvPipe;

// A buffer in the socket's memory
typedef struct SOCLSocketBuffer {
    u32 size;
    u8 *data; // NULL when size is 0
} SOCLSocketBuffer;

struct SOCLSocket;

// The send pipe, 0x110 bytes
typedef struct SOCLSendPipe {
    SOCLCommandPipe base;
    SOCLSocketBuffer buffer;
    volatile u16 write_pos;  // Where the next write goes in the buffer (a ring)
    volatile u16 read_pos;   // How far the pipe's thread has taken data out of it
    OSThreadQueue wait;      // Where a blocking write sleeps until the buffer has room
    struct SOCLSocket *socket; // The socket that owns the pipe (a UDP socket shares the UDP send socket's)
} SOCLSendPipe;

// A socket, 0xbc bytes, followed in memory by its pipes and buffers
typedef struct SOCLSocket {
    // The socket starts with its CPSSoc (0xa0 bytes: func_ov009_0215edd0 makes one on its own)
    u8 unk_00[8];
    u8 cps_state;              // 0x08, CPSSoc.state
    u8 unk_09[0x3f];
    SOCLSocketBuffer rcvbuf;   // 0x48
    u32 rcvbuf_len;            // CPSSoc.rcvbuf_len: how much of the receive buffer holds data
    SOCLSocketBuffer sndbuf;   // 0x54
    SOCLSocketBuffer rcvbuf_aux;
    SOCLSocketBuffer sndbuf_aux;
    u8 unk_6c[0x34];
    SOCLRecvPipe *recv_pipe;
    SOCLSendPipe *send_pipe;
    volatile int result; // What the last command packet returned
    // SOCLI_STATE_*. NitroWiFi declares state (vs16), result (vs32), the pipes' positions (vu16) and its UDP queue as
    // volatile: the command pipes' threads change them while callers test them
    volatile s16 state;
    s8 flag_block;
    s8 flag;    // The mode (SOCLSocketParam's flag_mode), copied into each command packet
    u16 local_port;  // 0xb0
    u16 remote_port;
    CPSInAddr remote_ip;
    struct SOCLSocket *next; // The next socket of the list it is in
} SOCLSocket;

// socl_startup.c: the socket that sends every UDP socket's datagrams
// pokebw2's SOCLiUDPSendSocket
extern SOCLSocket *data_ov009_021aa44c;

// func_ov009_0215d6c8's flags (BSD's MSG_PEEK and MSG_DONTWAIT)
#define SOCLI_READ_PEEK 2     // Leave the data to be read again
#define SOCLI_READ_NOBLOCK 4  // Do not wait for data

// func_ov009_0215e024's flags
#define SOCLI_WRITE_NOBLOCK 4 // Do not wait for room in the send buffer

// SOCLSocket.state
#define SOCLI_STATE_CREATED 1
#define SOCLI_STATE_BINDING 2        // func_ov009_0215d5f8 has queued the bind (and connect)
#define SOCLI_STATE_CONNECTED 4      // The bind (and for TCP the connect) succeeded
#define SOCLI_STATE_CLOSING 8
#define SOCLI_STATE_CLOSE_CALLED 0x10   // func_ov009_0215e84c has run (it queued the close command)
#define SOCLI_STATE_CLOSED 0x20         // The close command has run: the socket waits in the trash list
#define SOCLI_STATE_CONNECT_FAILED 0x40 // The connect failed, with the error in the socket's result
#define SOCLI_STATE_RESET 0x80       // The connection was reset

static inline BOOL SOCLi_SocketIsCreated(SOCLSocket *socket) {
    BOOL created = FALSE;
    if (socket != NULL) {
        if (socket->state & SOCLI_STATE_CREATED) {
            created = TRUE;
        } else {
            created = FALSE;
        }
    }
    return created;
}

// SOCLSocket.flag: 0 and 4 are TCP. SOCLi_ModeIsTCP tests a mode that was read already
static inline BOOL SOCLi_ModeIsTCP(s8 mode) {
    BOOL tcp = TRUE;
    if (mode != 0 && mode != 4) {
        tcp = FALSE;
    }
    return tcp;
}

static inline BOOL SOCLi_SocketIsTCP(SOCLSocket *socket) {
    return socket->flag == 0 || socket->flag == 4;
}

struct SOCLCommandPacket;
typedef int (*SOCLCommandHandler)(struct SOCLCommandPacket *packet);

// A command for a socket's pipe, 0x2c bytes: the handler, run by the pipe's thread, with its arguments after the
// header
typedef struct SOCLCommandPacket {
    SOCLCommandHandler handler;
    SOCLSocket *socket;
    OSMessageQueue *reply_queue; // Where a blocking caller waits for the result, NULL otherwise
    s8 socket_flag;
    s8 flag_block;               // 1 when the caller waits for the result (bit 0 of it blocks the send)
    u8 unk_0e[2];
    union {
        struct {
            u16 local_port;
            u16 remote_port;
            CPSInAddr remote_ip;
        } bind;
        struct {
            u8 *buf;
            int len;
            u16 *remote_port; // Where the datagram's sender goes, or NULL
            CPSInAddr *remote_ip;
        } read;
        struct {
            u8 *buf1;        // Where the data is in the send pipe's buffer, in up to two parts (the second when it
            int len1;        // wraps around the buffer's end)
            u8 *buf2;
            int len2;
            u16 write_end;   // The pipe's write position after this data
            u8 unk_22[2];
            u16 local_port;  // For a UDP packet, the datagram's ports and address
            u16 remote_port;
            CPSInAddr remote_ip; // 0 for TCP
        } write;
        struct {
            void *info; // What func_ov009_0215fc80 was given (an SSL connection), or NULL to turn SSL off
        } ssl;
        u8 raw[0x1c];
    } args;
} SOCLCommandPacket;

// socl_command.c
// pokebw2's SOCLi_StartupCommandPacketQueue
int func_ov009_0215cd3c(u32 max);
// pokebw2's SOCLi_CleanupCommandPacketQueue
int func_ov009_0215cdd0(void);
// pokebw2's SOCLi_AllocCommandPacket
SOCLCommandPacket *func_ov009_0215ce14(s32 flags);
// pokebw2's SOCLi_CreateCommandPacket
SOCLCommandPacket *func_ov009_0215ce3c(SOCLCommandHandler handler, SOCLSocket *socket, s32 flag_block);
// pokebw2's SOCLi_FreeCommandPacket
void func_ov009_0215ce7c(SOCLCommandPacket *packet);
// pokebw2's SOCLi_GetCtrlPipe
SOCLCommandPipe *func_ov009_0215ce9c(SOCLSocket *socket);
// pokebw2's SOCLi_SendCommandPacket
int func_ov009_0215ceb0(SOCLCommandPipe *pipe, SOCLCommandPacket *packet);
// pokebw2's SOCLi_SendCommandPacketToCtrlPipe
int func_ov009_0215cefc(SOCLSocket *socket, SOCLCommandPacket *packet);
// pokebw2's SOCLi_ExecCommandPacketInRecvPipe
int func_ov009_0215cf88(SOCLSocket *socket, SOCLCommandPacket *packet);
// pokebw2's SOCLi_ExecCommandPacketInSendPipe
int func_ov009_0215cf98(SOCLSocket *socket, SOCLCommandPacket *packet);
// pokebw2's SOCLi_ExecCommandPacketInCtrlPipe
int func_ov009_0215cfa8(SOCLSocket *socket, SOCLCommandPacket *packet);
// pokebw2's SOCLi_CommandPacketHandler
void func_ov009_0215cfc0(OSMessageQueue *queue);

// socl_bind.c
// pokebw2's SOCL_Bind
int func_ov009_0215d43c(int s, u16 local_port);
// pokebw2's SOCL_Connect
int func_ov009_0215d4ac(int s, u16 remote_port, CPSInAddr remote_ip);

// socl_write.c
// Writes to a socket; remote_port and remote_ip are for UDP (remote_ip 0 uses the socket's), flags is SOCLI_WRITE_*
// pokebw2's SOCL_WriteTo
int func_ov009_0215e024(SOCLSocket *socket, const u8 *buf, int len, u16 remote_port, CPSInAddr remote_ip, int flags);
// How much room the socket's send pipe has in its buffer
// pokebw2's SOCLi_GetWriteBufferFreeSize
int func_ov009_0215e2c0(const SOCLSocket *socket);

// socl_shutdown.c
// pokebw2's SOCL_Shutdown
int func_ov009_0215e6b8(SOCLSocket *socket);

// socl_close.c
// Whether a handle is no socket any more and has left the trash too
// pokebw2's SOCL_IsClosed
BOOL func_ov009_0215e794(int socket);
// TRUE for a handle that is no socket of the list and not in the trash, or that is a socket func_ov009_0215e84c has not
// run on (func_ov009_0215f644's caller in DWC asks it after a failed close; no counterpart in pokediamond's NitroWiFi)
BOOL func_ov009_0215e7c8(int socket);
// pokebw2's SOCL_Close
int func_ov009_0215e84c(int socket);
// pokebw2's SOCLi_TrashSocket
void func_ov009_0215ebb4(void);

// socl_cleanup.c
// pokebw2's SOCL_Cleanup
int func_ov009_0215ebf0(void);
// pokebw2's SOCL_Resolve
CPSInAddr func_ov009_0215edd0(const char *name);
// pokebw2's SOCL_InetAtoH
CPSInAddr func_ov009_0215ee98(const char *name);
// pokebw2's SOCL_GetHostID
CPSInAddr func_ov009_0215ef48(void);
// pokebw2's SOCL_SetResolver
int func_ov009_0215eee0(CPSInAddr dns1, CPSInAddr dns2);
// Shuts CPS down after a failed func_ov009_0215ca8c, and in func_ov009_0215ebf0 (no counterpart in pokediamond's
// NitroWiFi, whose func_ov009_0215ebf0 does it inline; name a guess)
// pokebw2's SOCLi_CleanupCPS
void func_ov009_0215ec90(void);

// socl_poll.c
// func_ov009_0215efb0's bits
#define SOCL_STATUS_READABLE 0x01  // Data to read (or a datagram)
#define SOCL_STATUS_WRITABLE 0x08  // Room in the send buffer
#define SOCL_STATUS_ERROR 0x20     // The connect failed
#define SOCL_STATUS_NOTCONN 0x40   // Neither connecting nor connected
#define SOCL_STATUS_INVALID 0x80   // Not a socket
// pokebw2's SOCL_GetStatus
int func_ov009_0215efb0(SOCLSocket *socket);

// socl_misc.c
// pokebw2's SOCLi_RoundUp4
u32 func_ov009_0215f0c8(u32 value);
// Two counters that func_ov009_0215d070 raises and func_ov009_0215e84c lowers, for the TCP sockets and for the others
// pokebw2's SOCLiSocketCount
extern s32 data_ov009_021aa4ac[2];
// Zeroes the socket counters before SOCL starts (no counterpart in pokediamond's NitroWiFi)
// pokebw2's SOCLi_InitResource
void func_ov009_0215f0d4(void);
// CPS's address check callback: drops a gateway or DNS server that the net mask and address rule out (no counterpart in
// pokediamond's NitroWiFi)
// pokebw2's SOCLi_CheckNConfig
BOOL func_ov009_0215f1d4(void);

// socl_read.c
// Reads from a socket; flags is SOCLI_READ_*, remote_port and remote_ip (either may be NULL) get the sender's
// pokebw2's SOCLi_ReadFrom
int func_ov009_0215d6c8(SOCLSocket *socket, u8 *buf, int len, u16 *remote_port, CPSInAddr *remote_ip, int flags);
// pokebw2's SOCLi_UdpRecvCallback
BOOL func_ov009_0215df08(u8 *data, u32 len, CPSSoc *soc);

// socl_list.c
// Puts a new socket in the list of sockets (func_ov009_0215f2dc in pokediamond's NitroWiFi)
// pokebw2's SOCLi_SocketRegister
void func_ov009_0215f2dc(SOCLSocket *socket);
// pokebw2's SOCL_SocketIsInvalid
BOOL func_ov009_0215f3d0(const SOCLSocket *socket); // TRUE for a handle that is not a socket of the list
// pokebw2's SOCL_SocketIsInTrash
BOOL func_ov009_0215f404(const SOCLSocket *socket); // TRUE for a socket that func_ov009_0215e980 put in the trash
// pokebw2's SOCLi_SocketUnregister
void func_ov009_0215f31c(SOCLSocket *socket);
// pokebw2's SOCLi_SocketRegisterTrash
void func_ov009_0215f304(SOCLSocket *socket);
// pokebw2's SOCLi_SocketUnregisterTrash
void func_ov009_0215f3b8(SOCLSocket *socket);
// The sockets that were closed and wait for func_ov009_0215ebb4 to free them
// pokebw2's SOCLiSocketList
extern SOCLSocket *data_ov009_021aa4b4;
// pokebw2's SOCLiSocketListTrash
extern SOCLSocket *data_ov009_021aa4b8;

#endif // BWPORT_NITROWIFI_SOCL_INTERNAL_H
