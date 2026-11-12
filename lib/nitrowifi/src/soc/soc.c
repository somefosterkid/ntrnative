// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/soc/soc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "nitrowifi/soc.h"
#include "socl_internal.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "gfl/std.h"
#include "dwc/ext.h"

// soc.c: the BSD-style SOC_ calls over SOCL, which NitroWiFi's SOC_ API is (pokediamond's soc.o: the same calls under
// the same names, in the reverse order, except that pokediamond calls the host name lookup SOC_GetHostByAddr, the
// name of the reverse lookup; pokeheartgold's func_ov009_0215f650 is the one for it). Their addresses and ports are in network order, so they swap them. The three
// tables of socket parameters (data_ov009_021a1ce4 and the others) are its data.

// How func_ov009_0215f428's sockets are made. A TCP socket gets no receive buffer of its own: it reads from CPS's.
// pokebw2's SOCLSocketParamTCP
SOCLSocketParam data_ov009_021a1cfc = {
    0, 1,
    { 0, 0, 0xb9e, 0, 0, 0x16d1, 0 },
    { 0x800, 12, 32 },
    { 0x800, 13, 32 },
};

// pokebw2's SOCLSocketParamUDP
SOCLSocketParam data_ov009_021a1ce4 = {
    1, 1,
    { 0x5c0, 1, 0, 0, 0, 0, 0x800 },
    { 0x800, 12, 32 },
    { 0, 0, 0 },
};

// pokebw2's SOCLSocketParamUDPSend
SOCLSocketParam data_ov009_021a1d14 = {
    2, 0,
    { 0, 0, 0x5ea, 0, 0, 0x6eb, 0 },
    { 0, 0, 0 },
    { 0x800, 13, 32 },
};

// The file's state, one .bss block from data_ov009_021aa4bc to the host name (-ipa file, library.toml, reaches it from one
// base). The SDK's statics are hostAddr (SOC_GetHostByName's hostip), the deallocator and allocator func_ov009_0215f8fc was
// given, the address list, the hostent, the inet_ntoa buffer, data_ov009_021aa50c and the host name (func_ov009_0215f650's entName,
// in a section of its own). The unk fields are the gaps Black's block has between them: statics of SDK functions this
// build leaves out (an unread static is dropped), kept here as padding inside two structs so that the block is laid out
// in size order as Black's is.
static u32 hostAddr;
static struct {
    SocFreeFunc freeFunc;       // 0x04
    u32 unk_08;
    SocAllocFunc allocFunc;     // 0x0c
    u32 unk_10[2];
    u8 *hostAddrList[2];        // 0x18
} data_ov009_021aa4c0;
static struct {
    SOCHostEnt hostEnt;         // 0x20
    char inetNtoABuffer[16];    // 0x30
    u32 unk_40[4];
} data_ov009_021aa4dc;
// pokebw2's soclConfig
static SOCLConfig data_ov009_021aa50c;   // 0x50

static void *func_ov009_0215f8a4(u32 size);
static void func_ov009_0215f8d0(void *ptr);

static inline u16 SOC_Swap16(u16 value) {
    return ((value >> 8) & 0xff) | ((value << 8) & 0xff00);
}

static inline u32 SOC_Swap32(u32 value) {
    return ((value >> 24) & 0xff) | ((value >> 8) & 0xff00) | ((value << 8) & 0xff0000) | ((value << 24) & 0xff000000);
}

// pokebw2's SOC_Socket
int func_ov009_0215f428(int domain, int type, int protocol) {
    if (type == POKE_NET_SOC_SOCK_STREAM) {
        return func_ov009_0215d070(&data_ov009_021a1cfc);
    }
    return func_ov009_0215d070(&data_ov009_021a1ce4);
}

// pokebw2's SOC_Bind
int func_ov009_0215f454(int sock, const SOCSockAddrIn *address) {
    return func_ov009_0215d43c(sock, SOC_Swap16(address->port));
}

// pokebw2's SOC_Connect
int func_ov009_0215f480(int sock, const SOCSockAddrIn *address) {
    return func_ov009_0215d4ac(sock, SOC_Swap16(address->port), SOC_Swap32(address->addr.addr));
}

// pokebw2's SOC_Read
int func_ov009_0215f4dc(int sock, void *buffer, int size, int flags) {
    return func_ov009_0215d6c8((SOCLSocket *)sock, buffer, size, NULL, NULL, flags);
}

// pokebw2's SOC_RecvFrom
int func_ov009_0215f500(int sock, void *buffer, int size, int flags, void *from) {
    u16 port;
    CPSInAddr ip;
    int result;

    result = func_ov009_0215d6c8((SOCLSocket *)sock, buffer, size, &port, &ip, flags);

    if (result >= 0 && from) {
        SOCSockAddrIn *addr = (SOCSockAddrIn *)from;
        addr->port = SOC_Swap16(port);
        addr->addr.addr = SOC_Swap32(ip);
    }
    return result;
}

// pokebw2's SOC_Send
int func_ov009_0215f590(int sock, const void *buffer, int size, int flags) {
    return func_ov009_0215e024((SOCLSocket *)sock, buffer, size, 0, 0, flags);
}

// pokebw2's SOC_SendTo
int func_ov009_0215f5b4(int sock, const void *buffer, int size, int flags, const SOCSockAddrIn *to) {
    u16 port;
    CPSInAddr ip;

    if (to != NULL) {
        port = SOC_Swap16(to->port);
        ip = SOC_Swap32(to->addr.addr);
    } else {
        port = 0;
        ip = 0;
    }
    return func_ov009_0215e024((SOCLSocket *)sock, buffer, size, port, ip, flags);
}

// pokebw2's SOC_Shutdown
int func_ov009_0215f638(int sock, int how) {
    return func_ov009_0215e6b8((SOCLSocket *)sock);
}

// pokebw2's SOC_Close
int func_ov009_0215f644(int sock) {
    return func_ov009_0215e84c(sock);
}

// pokebw2's SOC_GetHostByName
SOCHostEnt *func_ov009_0215f650(const char *host) {
    static char entName[0x101];
    CPSInAddr addr = func_ov009_0215edd0(host);

    if (addr == 0) {
        return NULL;
    }
    MI_CpuFill8(entName, 0, sizeof(entName));
    STD_CopyLString(entName, host, sizeof(entName));
    data_ov009_021aa4dc.hostEnt.name = entName;
    data_ov009_021aa4dc.hostEnt.aliases = NULL;
    data_ov009_021aa4dc.hostEnt.addrType = POKE_NET_SOC_AF_INET;
    data_ov009_021aa4dc.hostEnt.addrLength = 4;
    data_ov009_021aa4dc.hostEnt.addrList = data_ov009_021aa4c0.hostAddrList;
    data_ov009_021aa4c0.hostAddrList[0] = (u8 *)&hostAddr;
    data_ov009_021aa4c0.hostAddrList[1] = NULL;
    hostAddr = SOC_Swap32(addr);
    return &data_ov009_021aa4dc.hostEnt;
}

// pokebw2's SOC_GetSockName
int func_ov009_0215f714(int sock, SOCSockAddrIn *address) {
    CPSInAddr host_ip;
    u16 val;
    u32 port;

    if (((SOCLSocket *)sock) == NULL) {
        return SOCL_ENETRESET;
    }
    host_ip = func_ov009_0215ef48();
    val = ((SOCLSocket *)sock) != NULL ? ((SOCLSocket *)sock)->local_port : 0;
    port = val;
    if (host_ip == 0) {
        port = 0;
    }
    address->len = 8;
    address->family = POKE_NET_SOC_AF_INET;
    address->port = SOC_Swap16(port);
    address->addr.addr = SOC_Swap32(host_ip);
    return 0;
}

// pokebw2's SOC_GetHostID
u32 func_ov009_0215f7b0(void) {
    return SOC_Swap32(func_ov009_0215ef48());
}

// The SDK's name, which is wrong: it sets the DNS servers
// pokebw2's SOC_GetResolver
int func_ov009_0215f7e8(const u32 *dns1, const u32 *dns2) {
    return func_ov009_0215eee0(SOC_Swap32(*dns1), SOC_Swap32(*dns2));
}

// pokebw2's SOC_Fcntl
int func_ov009_0215f854(int sock, int command, int flags) {
    SOCLSocket *socket = (SOCLSocket *)sock;

    if (socket == NULL) {
        return -1;
    }
    switch (command) {
    case 3:
        if (socket->flag_block == 1) {
            return 0;
        }
        return 4;
    case 4:
        if (flags & 4) {
            socket->flag_block = 0;
        } else {
            socket->flag_block = 1;
        }
        break;
    }
    return 0;
}

// SOCL's allocator and deallocator, which keep the size of a block in the word before it and hand the user's
// func_ov009_0215f8fc allocator that size
// pokebw2's AllocFunc_SOCL
static void *func_ov009_0215f8a4(u32 size) {
    u32 total = size + 4;
    u32 *block = data_ov009_021aa4c0.allocFunc(0, total);

    if (block != NULL) {
        *block = total;
        block++;
    }
    return block;
}

// pokebw2's FreeFunc_SOCL
static void func_ov009_0215f8d0(void *ptr) {
    u32 *block = ptr;

    if (block != NULL) {
        block--;
        data_ov009_021aa4c0.freeFunc(0, block, *block);
    }
}

// pokebw2's SOC_Startup
int func_ov009_0215f8fc(const SOCConfig *config) {
    data_ov009_021aa50c.use_dhcp = config->flag == SOC_FLAG_DHCP ? TRUE : FALSE;
    data_ov009_021aa50c.host_ip.my_ip = SOC_Swap32(config->addr.addr);
    data_ov009_021aa50c.host_ip.net_mask = SOC_Swap32(config->netmask.addr);
    data_ov009_021aa50c.host_ip.gateway_ip = SOC_Swap32(config->router.addr);
    data_ov009_021aa50c.host_ip.dns_ip[0] = SOC_Swap32(config->dns1.addr);
    data_ov009_021aa50c.host_ip.dns_ip[1] = SOC_Swap32(config->dns2.addr);
    data_ov009_021aa50c.alloc = func_ov009_0215f8a4;
    data_ov009_021aa50c.free = func_ov009_0215f8d0;
    data_ov009_021aa4c0.allocFunc = config->alloc;
    data_ov009_021aa4c0.freeFunc = config->free;
    data_ov009_021aa50c.cmd_packet_max = 0x40;
    data_ov009_021aa50c.mtu = config->mtu;
    data_ov009_021aa50c.rwin = config->rwin;
    return func_ov009_0215ca8c(&data_ov009_021aa50c);
}

// pokebw2's SOC_Cleanup
int func_ov009_0215fa90(void) {
    return func_ov009_0215ebf0();
}

// pokebw2's SOC_InetNtoA
char *func_ov009_0215fa9c(SOCInAddr addr) {
    func_ov009_0215fb28(POKE_NET_SOC_AF_INET, &addr, data_ov009_021aa4dc.inetNtoABuffer, 16);
    return data_ov009_021aa4dc.inetNtoABuffer;
}

// pokebw2's SOC_InetAtoN
int func_ov009_0215fad0(const char *text, u32 *address) {
    CPSInAddr ip = func_ov009_0215ee98(text);

    if (ip == 0) {
        return 0;
    }
    if (address != NULL) {
        *address = SOC_Swap32(ip);
    }
    return 1;
}

// pokebw2's SOC_InetNtoP
char *func_ov009_0215fb28(int family, const void *address, char *buffer, u32 size) {
    u32 ip;
    u8 octets[4];

    if (family != POKE_NET_SOC_AF_INET) {
        return NULL;
    }
    if (size < 16) {
        return NULL;
    }
    MI_CpuCopy8(address, &ip, 4);
    func_ov009_0215fbac(ip, octets);
    OS_SNPrintf(buffer, 16, "%d.%d.%d.%d", octets[3], octets[2], octets[1], octets[0]);
    return buffer;
}

// pokebw2's SOC_U32to4U8
void func_ov009_0215fbac(u32 value, u8 *bytes) {
    bytes[0] = value >> 24;
    bytes[1] = value >> 16;
    bytes[2] = value >> 8;
    bytes[3] = value;
}

// pokebw2's SOC_Poll
int func_ov009_0215fbcc(SOCPollFD *fds, u32 count, s64 ticks) {
    BOOL timed;
    SOCPollFD *fd;
    u32 i;
    int ready;

    if (count == 0 || fds == NULL) {
        return 0;
    }
    timed = ticks != -1;
    while (1) {
        fd = fds;
        ready = 0;
        for (i = 0; i < count; i++, fd++) {
            int mask = fd->events | 0xe0;
            int status = func_ov009_0215efb0((SOCLSocket *)fd->fd) & mask;

            if (status != 0) {
                ready++;
            }
            fd->revents = status;
        }
        if (ready > 0) {
            break;
        }
        if (timed && ticks <= 0) {
            break;
        }
        OS_Sleep(1);
        ticks -= 523; // One millisecond of OS ticks
    }
    return ready;
}
