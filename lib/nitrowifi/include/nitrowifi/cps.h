// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2) at 9a3236bc8, lib/nitrowifi/include/nitrowifi/cps.h.
#ifndef BWPORT_NITROWIFI_CPS_H
#define BWPORT_NITROWIFI_CPS_H

#include "types.h"
#include "nitro/math.h"
#include "nitro/os.h"

// NitroWiFi's CPS, Ubiquitous's TCP/IP stack ("[SDK+UBIQUITOUS:CPS]"), in overlay 9 (cps.c). The ROM names none of
// it: the public calls and variables carry the names of NitroWiFi's CPS API where the code shows their role (the
// socket layer, SOCL, which follows in the overlay, calls them), and the internal ones descriptive CPSi_ names.
//
// A socket (CPSSoc) belongs to a thread: func_ov009_0215aa5c puts it in the thread's first specific word, and every
// CPS_Soc call works on the socket of the thread that makes it. Three threads of the stack's own run under it: one
// receives the frames that the wireless driver puts in the LAN buffer, one runs DHCP and the timeouts once a second,
// and one sends the delayed TCP ACKs when an alarm wakes it.

typedef u32 CPSInAddr;

// What func_ov009_021573c4 takes
typedef struct CPSConfig {
    u32 mode;                 // CPS_MODE_*
    void *(*alloc)(u32 size); // Both or neither
    void (*free)(void *ptr);
    void (*dhcp_callback)(void); // Called when DHCP gives up, or when the address is static
    BOOL (*link_is_on)(void);
    u64 random_seed; // 0 to seed with the tick count
    u8 *lan_buf;     // Where the received frames wait for the receiving thread
    u32 lan_buflen;
    u32 mymss;              // 0 for 1460
    CPSInAddr requested_ip; // The address DHCP asks for, or 0
    u32 yield_wait;         // Milliseconds that a wait sleeps for, or 0 to only yield
} CPSConfig;

// mode: an address of the config's own instead of DHCP's, and DNS servers of the config's own
#define CPS_MODE_STATIC_IP 1
#define CPS_MODE_STATIC_DNS 2

// Why there is no address (data_ov009_021a82b8)
enum {
    CPS_NOIP_REASON_NONE,
    CPS_NOIP_REASON_LINKOFF,
    CPS_NOIP_REASON_DHCPDISCOVERY,
    CPS_NOIP_REASON_LEASETIMEOUT,
    CPS_NOIP_REASON_COLLISION,
    CPS_NOIP_REASON_REJECTED,
};

// A socket's state
enum {
    CPS_STT_CLOSED,
    CPS_STT_LISTEN,
    CPS_STT_SYN_SENT,
    CPS_STT_SYN_RCVD,
    CPS_STT_ESTABLISHED,
    CPS_STT_UNUSED,
    CPS_STT_LAST_ACK, // The peer closed first and the FIN was answered
    CPS_STT_FIN_WAIT1,
    CPS_STT_FIN_WAIT2,
    CPS_STT_CLOSING,
    CPS_STT_DATAGRAM, // UDP
    CPS_STT_PING,     // ICMP echo
};

// What a socket's thread sleeps for
enum {
    CPS_BLOCK_NONE,
    CPS_BLOCK_TCPCON,
    CPS_BLOCK_TCPREAD,
    CPS_BLOCK_UDPREAD,
};

// A run of TCP data that arrived ahead of what the socket expects, at an offset in the receive buffer. order tells
// the most recent run
typedef struct CPSSocOoo {
    u32 start;
    u32 len;
    u32 order;
} CPSSocOoo;

typedef struct CPSSoc {
    OSThread *thread;
    u32 block_type;
    u8 state;
    u8 ssl;
    u16 local_port;
    void *con;               // The SSL connection
    s32 when;                // When the connection began, in units of 65536 ticks
    CPSInAddr local_ip_real; // The address the last datagram or SYN was sent to
    u16 remote_port;
    u16 remote_port_bound;
    CPSInAddr remote_ip;
    CPSInAddr remote_ip_bound;
    u32 ackno;        // The next sequence number expected
    u32 acked;        // The one the last ACK sent
    u32 ack_time;     // When that ACK went, in ticks
    u16 acked_window; // The window the last segment sent
    u8 delayed_ack;   // Segments that came in since
    u8 sack_permitted;
    u32 seqno; // The next sequence number to send
    u16 remote_win;
    u16 remote_mss;
    u32 remote_ackno;
    u32 segments;                                                // Segments received with data
    BOOL (*udp_callback)(u8 *data, u32 len, struct CPSSoc *soc); // TRUE when it took the datagram
    u32 rcvbuf_size;
    u8 *rcvbuf;
    u32 rcvbuf_len;
    u32 sndbuf_size;
    u8 *sndbuf;
    u8 unk_5c[0xc];
    u8 *outbuf; // Data that func_ov009_0215b470 sends ahead of the next write
    u32 outbuf_len;
    CPSSocOoo ooo[4];
} CPSSoc;

// An SSL connection (NitroWiFi's CPSSslConnection, 0x830 bytes): only its last fields, which the callers set, are
// written out yet
typedef struct CPSSslConnection {
    u8 unk_000[0x800];
    char *server_name; // The name the certificate must carry
    u8 *cert;
    int certlen;
    u32 cur_date;
    int (*auth_callback)(int result, struct CPSSslConnection *con, int level);
    void **ca_info;
    int ca_builtins;
    void *my_key;
    void *my_certificate;
    u8 *inbuf;
    long inbuf_len;
    long inbuf_pnt;
} CPSSslConnection;

// The stack's state that the socket layer reads
// pokebw2's CPSMode
extern u32 data_ov009_021a8278;  // : the config's mode
// pokebw2's CPSNetMask
extern CPSInAddr data_ov009_021a827c;
extern void *(*data_ov009_021a8280)(u32 size);
// pokebw2's CPSGatewayIp
extern CPSInAddr data_ov009_021a8298;
extern void (*data_ov009_021a82b0)(void *ptr);
// pokebw2's CPSNoIpReason
extern u32 data_ov009_021a82b8;
// pokebw2's CPSMyIp
extern CPSInAddr data_ov009_021a82bc;  // 0 until DHCP gives an address
// pokebw2's CPSDnsIp
extern CPSInAddr data_ov009_021a82dc[2];
// pokebw2's CPSiRand
extern MATHRandContext32 data_ov009_021a82e4;  // which the SSL layer draws from too

// CPS's signature, "[SDK+UBIQUITOUS:CPS]", which func_ov009_021573c4 refers to; the string sits in the main module at
// 0x020050ac
extern const char CPSi_Signature[];

// pokebw2's CPS_SetAddressCheckCallback
void func_ov009_02157280(BOOL (*callback)(void));  // : may reject a DHCP address
// pokebw2's CPS_Startup
void func_ov009_021573c4(CPSConfig *config);
// pokebw2's CPS_CalmDown
BOOL func_ov009_021575dc(void);  // : asks the DHCP thread to end, TRUE once it has
// pokebw2's CPS_SetScavengerCallback
void func_ov009_02157630(void (*callback)(void));  // : called once a second
// pokebw2's CPS_Cleanup
void func_ov009_02157640(void);
// pokebw2's CPS_SetThreadPriority
void func_ov009_021576c8(u32 priority);
// The wireless driver's receive callback, which puts a frame in the LAN buffer
// pokebw2's CPSi_RecvCallback
void func_ov009_02157c28(u8 *src, u8 *dst, u8 *data, u32 len);
// pokebw2's CPSi_GetNewPort
u16 func_ov009_0215a970(void);                                           // A free local port
// pokebw2's CPS_SocRegister
void func_ov009_0215aa5c(CPSSoc *soc);
// pokebw2's CPS_SocUnRegister
void func_ov009_0215aa70(void);
// pokebw2's CPS_SocDatagramMode
void func_ov009_0215aa88(void);
// pokebw2's CPS_SocBind
void func_ov009_0215aab0(u16 localPort, u16 remotePort, CPSInAddr remoteIp);
// pokebw2's CPS_SocUse
void func_ov009_0215ab10(void);
// pokebw2's CPS_SocRelease
void func_ov009_0215ab5c(void);
// pokebw2's CPS_SocDup
void func_ov009_0215ab7c(OSThread *thread);  // : gives a thread the caller's socket
// pokebw2's CPS_SetUdpCallback
void func_ov009_0215ab94(BOOL (*callback)(u8 *data, u32 len, CPSSoc *soc));
// pokebw2's CPSi_TcpConnect
int func_ov009_0215abb0(CPSSoc *soc);                                          // TCP's connect, 0 once connected
// pokebw2's CPS_TcpConnect
int func_ov009_0215ac60(void);
// pokebw2's CPS_TcpShutdown
void func_ov009_0215ad00(void);
// pokebw2's CPS_TcpClose
void func_ov009_0215ad3c(void);
// pokebw2's CPSi_TcpRead
u8 *func_ov009_0215ae18(u32 *len, CPSSoc *soc);                                   // TCP's read
// pokebw2's CPS_SocRead
u8 *func_ov009_0215aeb0(u32 *len);
// pokebw2's CPSi_TcpConsume
void func_ov009_0215af14(u32 len, CPSSoc *soc);                                // TCP's consume
// pokebw2's CPS_SocConsume
void func_ov009_0215b064(u32 len);
// pokebw2's CPSi_TcpWrite
u32 func_ov009_0215b198(u8 *data, u32 len, u8 *data2, u32 len2, CPSSoc *soc);    // TCP's write
// pokebw2's CPS_SocWrite
u32 func_ov009_0215b470(u8 *data, u32 len);
// pokebw2's CPS_SocGetLength
s32 func_ov009_0215b504(void);  // -1 once the connection is gone
// pokebw2's CPS_SocFlush
void func_ov009_0215b584(void);
// pokebw2's CPS_Resolve
CPSInAddr func_ov009_0215c910(const char *name);

// CPS's SSL layer, which follows in the overlay: a socket's calls go there when its ssl is set
// pokebw2's CPS_SslGetMaxPriority
u32 CPS_GetSslHandshakePriority(void);  // 's role
// pokebw2's CPS_SslSetMaxPriority
void CPS_SetSslHandshakePriority(u32 priority);  // 's role
// pokebw2's CPS_SslSetNamedParams
void CPS_SetRootCa(void *const *params, u32 count); // Registers the caller's named params (pinned
                                                            // certificates) on the current thread's connection
// pokebw2's CPS_SslAddRandomSeed
void func_ov009_02162248(const void *seed, u32 len);  // 's role: stirs entropy into set_random's pool
// pokebw2's CPS_SslConnect
int CPSi_SslConnect(CPSSoc *soc);
// pokebw2's CPS_SslRead
u8 *CPSi_SslRead(u32 *len, CPSSoc *soc);
// pokebw2's CPS_SslReadRelease
void CPSi_SslConsume(u32 len, CPSSoc *soc);
// pokebw2's CPS_SslCanRead
s32 CPSi_SslGetLength(CPSSoc *soc);
// pokebw2's CPS_SslWrite
u32 CPSi_SslWrite2(u8 *data, u32 len, u8 *data2, u32 len2, CPSSoc *soc);
// pokebw2's CPS_SslShutdown
void CPSi_SslShutdown(CPSSoc *soc);
// pokebw2's CPS_SslClose
void CPSi_SslClose(CPSSoc *soc);
// pokebw2's CPS_SslTick
void CPSi_SslPeriodical(s32 now);
// pokebw2's CPS_SslInit
void CPSi_SslCleanup(void);
// pokebw2's CPS_SetSsl
void CPS_SetSsl(BOOL enable);  // 's role: turns the SSL layer on for the caller's socket

#endif // BWPORT_NITROWIFI_CPS_H
