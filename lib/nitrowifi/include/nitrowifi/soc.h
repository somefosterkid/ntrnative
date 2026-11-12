// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2) at 9a3236bc8, lib/nitrowifi/include/nitrowifi/soc.h.
#ifndef BWPORT_NITROWIFI_SOC_H
#define BWPORT_NITROWIFI_SOC_H

#include "types.h"
#include "nitrowifi/cps.h"

// NitroWiFi's socket layer, SOCL, over CPS, in overlay 9, and the BSD-style SOC calls over it. The ROM names none of
// it. Its files and names follow NitroWiFi's objects as pret's pokediamond links them (socl_startup.o, socl_command.o
// and so on, with every function named), matched to this newer version (NitroWiFi 3.3, by its signature in the main
// module) by what each function does.

// What func_ov009_0215ca8c takes
typedef struct SOCLConfig {
    BOOL use_dhcp; // Ask DHCP for an address, or use host_ip
    struct {
        CPSInAddr my_ip;
        CPSInAddr net_mask;
        CPSInAddr gateway_ip;
        CPSInAddr dns_ip[2];
    } host_ip;
    void *(*alloc)(u32 size);
    void (*free)(void *ptr);
    u32 cmd_packet_max;   // How many command packets the sockets can have waiting
    u32 lan_buffer_size;  // 0 for SOCLiConfigState.lan_buffer_size_default
    void *lan_buffer;     // NULL to allocate lan_buffer_size bytes
    s32 cps_thread_prio;  // 0 for 11
    s32 mtu;              // 0 for 576, and kept between 576 and 1500
    s32 rwin;             // TCP's receive window, 0 for 0x10c0
} SOCLConfig;

// The thread of a socket's command pipe, and how many packets it queues
typedef struct SOCLPipeParam {
    u16 stack_size;
    u8 priority;
    u8 queue_length;
} SOCLPipeParam;

// How a socket is created: data_ov009_021a1cfc, data_ov009_021a1ce4 and data_ov009_021a1d14 (soc.c). A socket has a
// receive pipe when rcvbuf_size is not 0 and a send pipe when sndbuf_size is not 0; without one it uses the UDP send
// socket's.
typedef struct SOCLSocketParam {
    s8 flag_mode;
    s8 flag_block;
    struct {
        u16 rcvbuf_size;
        u16 rcvbuf_consume_min;
        u16 sndbuf_size;
        u16 rcvbuf_aux_size;
        u16 sndbuf_aux_size;
        u16 sndpipe_buf_size;
        u16 rcvpipe_unk_e;
    } buffer;
    SOCLPipeParam rcv_pipe;
    SOCLPipeParam snd_pipe;
} SOCLSocketParam;

// pokebw2's SOCLSocketParamUDP
extern SOCLSocketParam data_ov009_021a1ce4;
// pokebw2's SOCLSocketParamTCP
extern SOCLSocketParam data_ov009_021a1cfc;
// pokebw2's SOCLSocketParamUDPSend
extern SOCLSocketParam data_ov009_021a1d14;

// What SOCL_Startup was given, and the LAN buffer's size when the config gives none. Stands for NitroWiFi's
// SOCLiConfigPtr and SOCL_LAN_BUFFER_SIZE_DEFAULT, which are separate there: MWCC reaches them from one base here, and
// the zero ConfigPtr sits in .data beside the size, which a plain `= NULL` global does not (it goes to .bss)
typedef struct SOCLiConfigState {
    SOCLConfig *config; // NULL while SOCL is down
    u32 lan_buffer_size_default;
} SOCLiConfigState;

extern SOCLiConfigState data_ov009_021a1cd8;

// The DHCP state's bits (data_ov009_021aa448)
#define SOCL_DHCP_REQUEST 0x01  // DHCP is in use
#define SOCL_DHCP_CALLBACK 0x02 // CPS has called back: it has an address (or the static one), or DHCP gave up
#define SOCL_DHCP_ERROR SOCL_DHCP_CALLBACK

// pokebw2's SOCLiYieldWait: what CPSConfig's yield_wait gets
extern u32 data_ov009_021aa440;

// SOCL's errors, as negative return values (BSD's numbers)
#define SOCL_EAGAIN (-6)
#define SOCL_EALREADY (-7)
#define SOCL_ECANCELED (-11) // What the commands still queued in a pipe that is freed get
#define SOCL_ECONNRESET (-15)
#define SOCL_EINPROGRESS (-26)
#define SOCL_EINVAL (-28)
#define SOCL_EMFILE (-33)
#define SOCL_EMSGSIZE (-35) // A datagram that is more than a packet holds
#define SOCL_EISCONN (-30)
#define SOCL_ENETRESET (-39) // SOCL has not been started
#define SOCL_ENOMEM (-49)
#define SOCL_ENOTCONN (-56)
#define SOCL_ETIMEDOUT (-76)

// The BSD-style calls over SOCL (soc.c). A socket is the handle that func_ov009_0215f428 returns (SOCL's socket, which
// the calls take as an int), and they return negative SOCL_E* codes. The address and port of a socket address are in
// network order, and its first byte is its size
#define POKE_NET_SOC_AF_INET 2
#define POKE_NET_SOC_SOCK_STREAM 1
#define POKE_NET_SOC_SHUT_RDWR 2
#define POKE_NET_SOC_EWOULDBLOCK (-6)
#define POKE_NET_SOC_EINPROGRESS (-26)
#define POKE_NET_SOC_EISCONN (-30)

// Network order (big-endian) to the host's and back
#define SOC_NtoHl(netlong)                                                                                                 ((u32)((((u32)(netlong) >> 24) & 0x000000ff) | (((u32)(netlong) >> 8) & 0x0000ff00) |                                         (((u32)(netlong) << 8) & 0x00ff0000) | (((u32)(netlong) << 24) & 0xff000000)))
#define SOC_NtoHs(netshort) ((u16)((((u16)(netshort) >> 8) & 0x00ff) | (((u16)(netshort) << 8) & 0xff00)))
#define SOC_HtoNl(hostlong)                                                                                                ((u32)((((u32)(hostlong) >> 24) & 0x000000ff) | (((u32)(hostlong) >> 8) & 0x0000ff00) |                                       (((u32)(hostlong) << 8) & 0x00ff0000) | (((u32)(hostlong) << 24) & 0xff000000)))
#define SOC_HtoNs(hostshort) ((u16)((((u16)(hostshort) >> 8) & 0x00ff) | (((u16)(hostshort) << 8) & 0xff00)))

// An address (in network order) passed by value
struct SOCInAddr {
    u32 addr;
};

struct SOCSockAddrIn {
    u8 len;
    u8 family;
    u16 port;
    SOCInAddr addr;
};
// A socket address of any family, which an address in network order (SOCSockAddrIn) is one of
struct SOCSockAddr {
    u8 len;
    u8 family;
    u8 data[6];
};
struct SOCHostEnt {
    char *name;
    char **aliases;
    s16 addrType;
    s16 addrLength;
    u8 **addrList;
};
// A poll descriptor, and the configuration that func_ov009_0215f8fc (SOC_Startup) takes (its addresses are in network
// order)
typedef struct SOCPollFD {
    int fd;
    s16 events;
    s16 revents;
} SOCPollFD;
typedef void *(*SocAllocFunc)(u32 name, s32 size);
typedef void (*SocFreeFunc)(u32 name, void *ptr, s32 size);
#define SOC_VENDOR_NINTENDO 0x0000
#define SOC_VERSION 0x0100
#define SOC_FLAG_DHCP 0x0001 // SOCConfig's flag: get the addresses by DHCP
typedef struct SOCConfig {
    u16 vendor;         // 0x00, SOC_VENDOR_NINTENDO
    u16 version;        // 0x02, SOC_VERSION
    SocAllocFunc alloc; // 0x04
    SocFreeFunc free;   // 0x08
    u32 flag;           // 0x0c, SOC_FLAG_*
    SOCInAddr addr;     // 0x10
    SOCInAddr netmask;  // 0x14
    SOCInAddr router;   // 0x18
    SOCInAddr dns1;     // 0x1c
    SOCInAddr dns2;     // 0x20
    s32 timeWaitBuffer;
    s32 reassemblyBuffer;
    s32 mtu;  // 0x2c: SOCLConfig's mtu
    s32 rwin; // 0x30: SOCLConfig's rwin
    s64 r2;   // 0x34
    const char *peerid;
    const char *passwd;
    const char *serviceName;
    const char *hostName; // 0x48
    s32 rdhcp;
    s32 udpSendBuff;
    s32 udpRecvBuff;
} SOCConfig;

// pokebw2's SOC_Socket
int func_ov009_0215f428(int domain, int type, int protocol);
// pokebw2's SOC_Bind
int func_ov009_0215f454(int sock, const SOCSockAddrIn *address);
// pokebw2's SOC_Connect
int func_ov009_0215f480(int sock, const SOCSockAddrIn *address);
// pokebw2's SOC_Read
int func_ov009_0215f4dc(int sock, void *buffer, int size, int flags);
// pokebw2's SOC_RecvFrom
int func_ov009_0215f500(int sock, void *buffer, int size, int flags, void *from);
// pokebw2's SOC_Send
int func_ov009_0215f590(int sock, const void *buffer, int size, int flags);
// pokebw2's SOC_SendTo
int func_ov009_0215f5b4(int sock, const void *buffer, int size, int flags, const SOCSockAddrIn *to);
// pokebw2's SOC_Shutdown
int func_ov009_0215f638(int sock, int how);
// pokebw2's SOC_Close
int func_ov009_0215f644(int sock);
// pokebw2's SOC_GetHostByName
SOCHostEnt *func_ov009_0215f650(const char *host);
// pokebw2's SOC_GetSockName
int func_ov009_0215f714(int sock, SOCSockAddrIn *address);
// pokebw2's SOC_GetHostID
u32 func_ov009_0215f7b0(void);
// Sets the DNS servers, despite the name (the SDK's; pokediamond's is the same)
// pokebw2's SOC_GetResolver
int func_ov009_0215f7e8(const u32 *dns1, const u32 *dns2);
// pokebw2's SOC_Fcntl
int func_ov009_0215f854(int sock, int command, int flags);
// pokebw2's SOC_Startup
int func_ov009_0215f8fc(const SOCConfig *config);
// pokebw2's SOC_Cleanup
int func_ov009_0215fa90(void);
// pokebw2's SOC_InetNtoA
char *func_ov009_0215fa9c(SOCInAddr addr);
// pokebw2's SOC_InetAtoN
int func_ov009_0215fad0(const char *text, u32 *address);
// pokebw2's SOC_InetNtoP
char *func_ov009_0215fb28(int family, const void *address, char *buffer, u32 size);
// pokebw2's SOC_U32to4U8
void func_ov009_0215fbac(u32 value, u8 *bytes);
// pokebw2's SOC_Poll
int func_ov009_0215fbcc(SOCPollFD *fds, u32 count, s64 ticks);
// pokebw2's SOCL_EnableSsl
int func_ov009_0215fc80(int sock, void *sslInfo);
// pokebw2's SOCLi_TrashSocket
void func_ov009_0215ebb4(void);

// pokebw2's SOCL_Startup
int func_ov009_0215ca8c(SOCLConfig *config);
// pokebw2's SOCL_CreateSocket
int func_ov009_0215d070(const SOCLSocketParam *param);
// Asks the sockets to close; SOCL_EINPROGRESS (nonzero) while it has not finished
int SOCL_CalmDown(void);

#endif // BWPORT_NITROWIFI_SOC_H
