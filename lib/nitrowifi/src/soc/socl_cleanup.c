// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/soc/socl_cleanup.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "nitrowifi/soc.h"
#include "socl_internal.h"
#include "dwc/wcm.h"
#include "gfl/std.h"
#include "nitro/os.h"

// SOCL's shutdown and its address calls: func_ov009_0215ebf0 waits for SOCL_CalmDown to close every socket (func_ov009_0215ecb8,
// then the UDP send socket) and ends CPS. The resolver and host address calls follow. pokediamond's NitroWiFi names the
// object socl_cleanup.o and its functions; this version split CPS's shutdown out of func_ov009_0215ebf0.

static int func_ov009_0215ecb8(void);

// pokebw2's SOCL_Cleanup
int func_ov009_0215ebf0(void) {
    int result;

    if (data_ov009_021aa444 == 0) {
        data_ov009_021aa444 = data_ov009_021a82bc;
    }
    if (SOCL_CalmDown() == SOCL_EINPROGRESS) {
        do {
            OS_Sleep(100);
        } while (SOCL_CalmDown() == SOCL_EINPROGRESS);
    }
    result = func_ov009_0215cdd0();
    if (result >= 0) {
        func_ov009_0215ec90();
        if (data_ov009_021a1cd8.config->lan_buffer == NULL) {
            data_ov009_021a1cd8.config->free(data_ov009_021aa450.lan_buf);
        }
        data_ov009_021a1cd8.config = NULL;
        func_ov009_0215f0d4();
    }
    return result;
}

// pokebw2's SOCLi_CleanupCPS
void func_ov009_0215ec90(void) {
    func_ov009_02157640();
    func_ov009_02157630(NULL);
    func_ov009_02157280(NULL);
    func_ov009_02166424(NULL);
}

// Closes the sockets that func_ov009_0215e84c has not run on, except the UDP send socket; EINPROGRESS while any is left
// pokebw2's SOCL_CloseAll
static int func_ov009_0215ecb8(void) {
    SOCLSocket *socket;
    u32 irq;

    while (TRUE) {
        irq = OS_DisableInterrupts();
        for (socket = data_ov009_021aa4b4; socket != NULL; socket = socket->next) {
            if (socket != data_ov009_021aa44c && !(socket->state & SOCLI_STATE_CLOSE_CALLED)) {
                break;
            }
        }
        OS_RestoreInterrupts(irq);
        if (socket == NULL) {
            break;
        }
        func_ov009_0215e84c((int)socket);
    }
    if (data_ov009_021aa4b4 == NULL || (data_ov009_021aa4b4 == data_ov009_021aa44c && data_ov009_021aa4b4->next == NULL)) {
        if (data_ov009_021aa4b8 == NULL) {
            return 0;
        }
    }
    return SOCL_EINPROGRESS;
}

// pokebw2's SOCL_CalmDown
int SOCL_CalmDown(void) {
    int result;

    if (data_ov009_021aa44c != NULL) {
        result = func_ov009_0215ecb8();
        if (result == 0) {
            func_ov009_0215e84c((int)data_ov009_021aa44c);
            if (func_ov009_0215e794((int)data_ov009_021aa44c)) {
                data_ov009_021aa44c = NULL;
            }
            result = SOCL_EINPROGRESS;
        }
        func_ov009_0215ebb4();
    } else if (func_ov009_021575dc()) {
        func_ov009_02166424(NULL);
        result = 0;
    } else {
        result = SOCL_EINPROGRESS;
    }
    return result;
}

// Asks the DNS servers of CPS for an address, on a socket of its own
// pokebw2's SOCL_Resolve
CPSInAddr func_ov009_0215edd0(const char *name) {
    CPSSoc soc;
    u8 *buf;
    CPSInAddr ip;

    if (data_ov009_021a1cd8.config == NULL) {
        return 0;
    }
    if (name == NULL) {
        return 0;
    }
    buf = data_ov009_021a1cd8.config->alloc(0x1708);
    if (buf == NULL) {
        return 0;
    }
    MI_CpuFill8(&soc, 0, sizeof(CPSSoc));
    soc.rcvbuf = buf;
    soc.rcvbuf_size = 0xb68;
    soc.sndbuf = buf + 0xb68;
    soc.sndbuf_size = 0xb9e;
    func_ov009_0215aa5c(&soc);
    ip = func_ov009_0215c910(name);
    func_ov009_0215aa70();
    data_ov009_021a1cd8.config->free(buf);
    return ip;
}

// Resolves a dotted address only: the DNS servers are cleared meanwhile, so func_ov009_0215c910 does not ask them
// pokebw2's SOCL_InetAtoH
CPSInAddr func_ov009_0215ee98(const char *name) {
    CPSInAddr dns1, dns2;
    CPSInAddr ip;
    u32 irq = OS_DisableInterrupts();

    dns1 = data_ov009_021a82dc[0];
    dns2 = data_ov009_021a82dc[1];
    data_ov009_021a82dc[0] = 0;
    data_ov009_021a82dc[1] = 0;
    ip = func_ov009_0215c910(name);
    data_ov009_021a82dc[0] = dns1;
    data_ov009_021a82dc[1] = dns2;
    OS_RestoreInterrupts(irq);
    return ip;
}

// pokebw2's SOCL_SetResolver
int func_ov009_0215eee0(CPSInAddr dns1, CPSInAddr dns2) {
    if (func_ov009_0215ef48() == 0) {
        return SOCL_ENETRESET;
    }
    data_ov009_021a82dc[0] = dns1;
    if (dns1 == 0) {
        data_ov009_021a8278 &= ~CPS_MODE_STATIC_DNS;
        data_ov009_021a82dc[1] = 0;
    } else {
        data_ov009_021a8278 |= CPS_MODE_STATIC_DNS;
        data_ov009_021a82dc[1] = dns2;
    }
    return 0;
}

// The host address, 0 while DHCP has none (the caller waits a little when DHCP is still running)
// pokebw2's SOCL_GetHostID
CPSInAddr func_ov009_0215ef48(void) {
    CPSInAddr ip = data_ov009_021a82bc;

    if (ip == 0) {
        if ((data_ov009_021aa448 & (SOCL_DHCP_REQUEST | SOCL_DHCP_CALLBACK)) == SOCL_DHCP_REQUEST) {
            if (OS_GetProcMode() != 0x12) {
                OS_Sleep(10);
            }
        }
    } else if (data_ov009_021aa444 == 0) {
        data_ov009_021aa444 = ip;
    }
    return data_ov009_021a82bc;
}
