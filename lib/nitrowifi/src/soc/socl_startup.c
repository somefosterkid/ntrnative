// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/soc/socl_startup.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "nitrowifi/soc.h"
#include "socl_internal.h"
#include "dwc/wcm.h"
#include "gfl/std.h"
#include "nitro/os.h"

// SOCL's startup: it starts CPS with a configuration made from SOCL's, then the command packet queue and the socket
// that sends UDP datagrams. pokediamond's NitroWiFi names the object socl_startup.o and every function in it.
static int func_ov009_0215cae4(void);
static void func_ov009_0215cb24(void);
static void func_ov009_0215cca0(void);
static void func_ov009_0215cd0c(void);
static BOOL func_ov009_0215cd24(void);

// NitroWiFi's signature, "[SDK+NINTENDO:WiFi3.3.30052.201006221129]", in the main module
extern const char SOCLi_Signature[];

// pokebw2's SOCLiConfigPtr
SOCLiConfigState data_ov009_021a1cd8 = { NULL, 0x4000 };

// pokebw2's SOCLiYieldWait
u32 data_ov009_021aa440;
// pokebw2's SOCLiUDPSendSocket
SOCLSocket *data_ov009_021aa44c;
// pokebw2's SOCLiRequestedIP
CPSInAddr data_ov009_021aa444;
// pokebw2's SOCLiDhcpState
u32 data_ov009_021aa448;
// pokebw2's SOCLiCPSConfig
CPSConfig data_ov009_021aa450;

// pokebw2's SOCL_Startup
int func_ov009_0215ca8c(SOCLConfig *config) {
    int result;

    OSi_ReferSymbol(SOCLi_Signature);
    if (data_ov009_021a1cd8.config != NULL) {
        return 0;
    }
    func_ov009_0215f0d4();
    data_ov009_021a1cd8.config = config;
    func_ov009_0215cb24();
    result = func_ov009_0215cae4();
    if (result < 0) {
        func_ov009_0215ec90();
        data_ov009_021a1cd8.config = NULL;
    }
    return result;
}

// pokebw2's SOCLi_StartupSOCL
static int func_ov009_0215cae4(void) {
    int result = func_ov009_0215cd3c(data_ov009_021a1cd8.config->cmd_packet_max);

    if (result >= 0) {
        data_ov009_021aa44c = (SOCLSocket *)func_ov009_0215d070(&data_ov009_021a1d14);
    }
    return result;
}

// pokebw2's SOCLi_StartupCPS
static void func_ov009_0215cb24(void) {
    CPSConfig *cps = &data_ov009_021aa450;
    SOCLConfig *config = data_ov009_021a1cd8.config;
    s32 mtu;
    s32 rwin;

    MI_CpuFill8(cps, 0, sizeof(CPSConfig));
    data_ov009_021aa450.alloc = config->alloc;
    data_ov009_021aa450.free = config->free;
    data_ov009_021aa450.link_is_on = func_ov009_0215cd24;
    data_ov009_021aa450.random_seed = 0;
    data_ov009_021aa450.yield_wait = data_ov009_021aa440;
    if (config->lan_buffer_size) {
        cps->lan_buflen = config->lan_buffer_size;
    } else {
        cps->lan_buflen = data_ov009_021a1cd8.lan_buffer_size_default;
    }
    if (config->lan_buffer) {
        cps->lan_buf = config->lan_buffer;
    } else {
        cps->lan_buf = SOCLi_Alloc(cps->lan_buflen);
    }

    mtu = config->mtu;
    if (mtu == 0) {
        mtu = 576;
    }
    if (mtu > 1500) {
        mtu = 1500;
    } else if (mtu < 576) {
        mtu = 576;
    }
    rwin = config->rwin;
    if (rwin == 0) {
        rwin = 0x10c0;
    }
    cps->mymss = mtu - 40;
    data_ov009_021a1cfc.buffer.rcvbuf_size = rwin;
    data_ov009_021a1cfc.buffer.rcvbuf_consume_min = cps->mymss * 2;

    data_ov009_021a82bc = 0;
    if (config->use_dhcp) {
        cps->mode = 0;
        data_ov009_021aa448 = SOCL_DHCP_REQUEST;
        cps->dhcp_callback = func_ov009_0215cd0c;
        cps->requested_ip = data_ov009_021aa444;
    } else {
        data_ov009_021aa448 = 0;
        cps->mode = CPS_MODE_STATIC_IP;
        cps->dhcp_callback = func_ov009_0215cca0;
    }
    func_ov009_021576c8(config->cps_thread_prio ? config->cps_thread_prio : 11);
    func_ov009_02166424((WCMRecvInd)func_ov009_02157c28);
    func_ov009_02157630(func_ov009_0215ebb4);
    func_ov009_02157280(func_ov009_0215f1d4);
    func_ov009_021573c4(cps);
}

// The address of the config's own, which CPS calls for when it does without DHCP
// pokebw2's SOCLi_SetMyIP
static void func_ov009_0215cca0(void) {
    SOCLConfig *config = data_ov009_021a1cd8.config;

    data_ov009_021a82bc = config->host_ip.my_ip;
    data_ov009_021a827c = config->host_ip.net_mask;
    data_ov009_021a8298 = config->host_ip.gateway_ip;
    data_ov009_021a82dc[0] = config->host_ip.dns_ip[0];
    data_ov009_021a82dc[1] = config->host_ip.dns_ip[1];
    data_ov009_021aa448 |= SOCL_DHCP_CALLBACK;
}

// pokebw2's SOCLi_DhcpTimeout
static void func_ov009_0215cd0c(void) {
    data_ov009_021aa448 |= SOCL_DHCP_CALLBACK | SOCL_DHCP_ERROR;
}

// pokebw2's SOCL_LinkIsOn
static BOOL func_ov009_0215cd24(void) {
    if (func_ov009_02166384() != NULL) {
        return TRUE;
    }
    return FALSE;
}
