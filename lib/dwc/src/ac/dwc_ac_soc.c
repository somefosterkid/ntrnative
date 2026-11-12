// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/src/ac/dwc_ac_soc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// DWC's auto-connect, the sockets (TWL-DWC 5.3, ac). The file name is a guess: its functions run the states that
// follow the search, which dwc_ac.c dispatches. State 1 starts the wireless manager, and the states 10 to 15 bring the
// sockets up over the connection to the access point (func_ov009_0215f8fc, the address from DHCP, the DNS servers, a test of
// the connection) or down again. The configuration that func_ov009_0215f8fc gets is made from the setting that the connection
// is with, and from the memory functions of auto-connect.
//
// The DWCAc_ names are ours, taken from what the code does.

#include "dwc/ac.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitrowifi/wcm.h"

static u8 ConnectTestStart(DWCAcSearchWork *work);
static u8 func_0205f1b8(DWCAcSearchWork *work);
static u8 ConnectTestCreate(void);
static u8 ConnectTestProcess(DWCAcSearchWork *work);
static u8 func_0205f35c(DWCAcSearchWork *work);
static u8 ConnectTestRetry(void);
static void MakeSOCConfig(DWCAcMain *main, DWCAcSearchWork *work, SOCConfig *config);
static u32 ConvAddress(const u8 *address);
static u32 ConvNetMask(u8 bits);
static void CheckSetDNS(DWCAcSearchWork *work);

// The name that the console gives DHCP (the word before the 4, in the unknown part of the configuration, points to it)
// pokebw2's sSocHostName
static char data_020a8130[] = "NINTENDO-DS";

// What MakeSOCConfig starts the configuration of func_ov009_0215f8fc from: DHCP on, the addresses, the allocators, the
// MTU and the window left to the function, then the two words of 0x1000 and the host name. The flag and these words are
// not known
// pokebw2's sSocConfigInit
static const SOCConfig data_020a16a0 = {
    SOC_VENDOR_NINTENDO, SOC_VERSION, NULL, NULL, SOC_FLAG_DHCP, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, 0x1000, 0x1000, 0, 0, 0,
    NULL, NULL, NULL, data_020a8130, 4, 0, 0,
};

// State 1, starts the wireless manager with the notification callback of dwc_ac_wcm.c, after
// listing the targets of the first search. Returns the state that follows: 2, the search; 0x11, the teardown, if the
// wireless manager will not start; and 1, to wait, while it is not in its first state
// pokebw2's DWCAc_StepWcmStartup
u8 DWCi_AC_Start(void) {
    WCMConfig config;
    s32 wcmState = func_ov009_021656a0();
    DWCAcSearchWork *work = func_0205c214(DWC_AC_WORK_SEARCH);
    s32 result;

    if (wcmState == 1) {
        config.dmano = work->dmaNo;
        config.pbdbuffer = NULL;
        config.nbdbuffer = 0;
        config.nbdmode = 0;
        func_0205d318(0);
        result = func_ov009_02164f98(&config, func_0205b148);
        if (result == 1 || result >= 4) {
            DWCi_AC_SetError(1);
            return 0x11;
        }
    } else {
        return 1;
    }
    return 2;
}

// The states 10 to 15. While the wireless manager holds the connection (its state 9), each does its
// part: 10 starts SOC, 11 takes it down, 12 waits for the address, 13 starts the connection test, 14 waits for it and
// 15 notes the connection. Once the connection is gone, 11 and 15 still go on as they are, 14 lets the test go first,
// and every state marks the candidate as lost and goes to 11, to take SOC down. Returns the state that follows
// pokebw2's DWCAc_StepSockets
u8 DWCi_AC_ConnectTest(void) {
    // The cases of both switches are in the order that the ROM lays out their bodies in
    u8 state = DWCi_AC_GetPhase();
    DWCAcSearchWork *work = func_0205c214(DWC_AC_WORK_SEARCH);

    if (func_ov009_021656a0() == 9) {
        switch (state) {
        case 10:
            state = ConnectTestStart(work);
            break;
        case 12:
            state = func_0205f1b8(work);
            break;
        case 13:
            state = ConnectTestCreate();
            break;
        case 14:
            state = ConnectTestProcess(work);
            break;
        case 15:
            state = func_0205f35c(work);
            break;
        case 11:
            state = ConnectTestRetry();
            break;
        }
    } else {
        switch (state) {
        case 15:
            state = func_0205f35c(work);
            break;
        case 11:
            state = ConnectTestRetry();
            break;
        case 14:
            DWC_Netcheck_Abort();
            DWC_Netcheck_Destroy();
            // fall through to the loss of the candidate
        default:
            work->infos[work->selected].result = DWC_AC_RESULT_LOST;
            state = 11;
            break;
        }
    }
    return state;
}

// State 10, starts SOC from the configuration that MakeSOCConfig makes, with the wait of SOCL's
// yields at 4. 12 to wait for the address, or 0x11, the teardown, if SOC will not start
// pokebw2's DWCAc_StartSoc
static u8 ConnectTestStart(DWCAcSearchWork *work) {
    DWCAcMain *main = func_0205c214(DWC_AC_WORK_MAIN);
    SOCConfig *config = func_0205c214(DWC_AC_WORK_SOC_CONFIG);

    MakeSOCConfig(main, work, config);
    data_ov009_021aa440 = 4;
    if (func_ov009_0215f8fc(config) == 0) {
        return 12;
    }
    DWCi_AC_SetError(2);
    return 0x11;
}

// State 12, waits for the address. When it has come, gives SOC the DNS servers of the setting, and
// goes on to the connection test (13), or skips it (15) if the game asked for that or the setting on a DSi uses a
// proxy. If the address has not come in 10 seconds the candidate is given up, and SOC goes down (11)
// pokebw2's DWCAc_WaitHostId
static u8 func_0205f1b8(DWCAcSearchWork *work) {
    u8 type;

    if (func_ov009_0215f7b0() != 0) {
        CheckSetDNS(work);
        if (work->skipConnectionTest == 1) {
            return 15;
        }
        type = func_0205c4c8(work->kind);
        if (type >= 10 && type <= 12) {
            if (OS_IsRunOnTwl() && work->dsiSettings[(u8)(type - 10)].extra.unk_82 != 0) {
                return 15;
            }
        }
        return 13;
    }
    if (OS_TicksToSeconds(OS_GetTick() - work->connectedTime) < 10) {
        return 12;
    }
    work->infos[work->selected].result = DWC_AC_RESULT_GAVE_UP;
    return 11;
}

// State 13, starts the connection test of overlay 11 with the memory functions. 14 to wait
// for it, or 0x11, the teardown, if it will not start
// pokebw2's DWCAc_StartConnectionTest
static u8 ConnectTestCreate(void) {
    if (DWC_Netcheck_Create(func_0205c214(DWC_AC_WORK_MEMFUNCS)) == 0) {
        return 14;
    }
    DWCi_AC_SetError(3);
    return 0x11;
}

// State 14, waits for the connection test. Once it is over, takes its result for the error
// if the candidate is of the type that the game was told of, and lets the test go. 15 if it went well; else the
// candidate is given up, with a note if CPS found its address already in use, and SOC goes down (11)
// pokebw2's DWCAc_WaitConnectionTest
static u8 ConnectTestProcess(DWCAcSearchWork *work) {
    DWCAcMain *main = func_0205c214(DWC_AC_WORK_MAIN);
    u32 status = DWC_Netcheck_GetError();

    if (status != 0) {
        if (main->candidateType == func_0205c4c8(work->kind)) {
            main->testResult = DWC_Netcheck_GetReturnCode();
        }
        DWC_Netcheck_Destroy();
        if (status != 11) {
            work->infos[work->selected].result = DWC_AC_RESULT_GAVE_UP;
            if (data_ov009_021a82b8 == CPS_NOIP_REASON_COLLISION) {
                main->addressInUse = 1;
            } else {
                main->addressInUse = 0;
            }
            return 11;
        } else {
            return 15;
        }
    } else {
        return 14;
    }
}

// State 15, notes the type of the access point and the key mode of the setting that the
// connection is with. 16, the connection is up
// pokebw2's DWCAc_NoteConnected
static u8 func_0205f35c(DWCAcSearchWork *work) {
    func_0205c31c(work->kind);
    func_0205c3b0(work->kind);
    return 16;
}

// State 11, takes the sockets down: SOCL first, then SOC. 9, the end of the candidate, once they are
// down, else 11
// pokebw2's DWCAc_StopSoc
static u8 ConnectTestRetry(void) {
    int result;

    if (SOCL_CalmDown() != 0) {
        return 11;
    }
    result = func_ov009_0215fa90();
    if (result == 0 || result == SOCL_ENETRESET) {
        return 9;
    }
    return 11;
}

// Makes the configuration for func_ov009_0215f8fc from the setting that the connection is with: the
// allocators of auto-connect; the address, the mask, the gateway and the DNS servers if the setting has an address of
// its own, else DHCP; the MTU, which a DSi takes from the setting within 576 to 1500 (1400 if it has none); and on a
// DSi the receive window of the search's work, unless the MTU is the smallest. The allocators have the type of DWC's
// (size unsigned) where SOC wants a size with a sign
// pokebw2's DWCAc_MakeSocConfig
static void MakeSOCConfig(DWCAcMain *main, DWCAcSearchWork *work, SOCConfig *config) {
    DWCMemPage *setting;
    u8 type;

    MI_CpuCopy8(&data_020a16a0, config, sizeof(SOCConfig));
    config->alloc = (SocAllocFunc)main->alloc;
    config->free = (SocFreeFunc)main->free;
    if (work->kind >= 0x12) {
        return;
    }
    type = func_0205c4c8(work->kind);
    if (type <= 2) {
        setting = &work->userInfo[type];
    } else {
        setting = &work->dsiSettings[type - 10].ntr;
    }
    if (setting->ap.ip[0] != 0) {
        config->flag = 0;
        config->addr.addr = ConvAddress(setting->ap.ip);
        config->netmask.addr = ConvNetMask(setting->ap.netmask);
        config->router.addr = ConvAddress(setting->ap.gateway);
        config->dns1.addr = ConvAddress(setting->ap.dns[0]);
        config->dns2.addr = ConvAddress(setting->ap.dns[1]);
    } else {
        config->flag = SOC_FLAG_DHCP;
        config->addr.addr = 0;
        config->netmask.addr = 0;
        config->router.addr = 0;
        config->dns1.addr = 0;
        config->dns2.addr = 0;
    }
    if (OS_IsRunOnTwl()) {
        if (setting->ap.mtu == 0) {
            config->mtu = 0x578;
        } else if (setting->ap.mtu > 0x5dc) {
            config->mtu = 0x5dc;
        } else if (setting->ap.mtu < 0x240) {
            config->mtu = 0x240;
        } else {
            config->mtu = setting->ap.mtu;
        }
    } else {
        config->mtu = 0x240;
    }
    if (OS_IsRunOnTwl()) {
        if (setting->ap.mtu == 0x240) {
            config->rwin = 0;
        } else {
            config->rwin = work->rwin;
        }
    } else {
        config->rwin = 0;
    }
}

// An IPv4 address of four bytes in a setting (the first byte is the highest) as func_ov009_0215f8fc wants
// it, with its bytes in the other order
// pokebw2's DWCAc_ReadAddress
static u32 ConvAddress(const u8 *address) {
    u32 value = 0;

    value |= address[0] << 24;
    value |= address[1] << 16;
    value |= address[2] << 8;
    value |= address[3];
    return ((value >> 24) & 0xff) | ((value >> 8) & 0xff00) | ((value << 8) & 0xff0000) | ((value << 24) & 0xff000000);
}

// The netmask with that many of its high bits set, as func_ov009_0215f8fc wants it, with its bytes in the
// other order
// pokebw2's DWCAc_MakeNetmask
static u32 ConvNetMask(u8 bits) {
    u32 mask = 0xffffffff;
    int i;

    for (i = 0; i < 32 - bits; i++) {
        mask <<= 1;
    }
    return ((mask >> 24) & 0xff) | ((mask >> 8) & 0xff00) | ((mask << 8) & 0xff0000) | ((mask << 24) & 0xff000000);
}

// Gives SOC the DNS servers of the setting that the connection is with, when the address comes from
// DHCP but the setting has DNS servers of its own (those of a setting with an address of its own are in the
// configuration already)
// pokebw2's DWCAc_SetResolver
static void CheckSetDNS(DWCAcSearchWork *work) {
    DWCMemPage *setting;
    u32 dns1;
    u32 dns2;
    int dnsSum;
    u8 type;

    if (work->kind >= 0x12) {
        return;
    }
    type = func_0205c4c8(work->kind);
    if (type <= 2) {
        setting = &work->userInfo[type];
    } else {
        setting = &work->dsiSettings[type - 10].ntr;
    }
    dnsSum = setting->ap.dns[0][0] + setting->ap.dns[0][1] + setting->ap.dns[0][2] + setting->ap.dns[0][3];
    if (setting->ap.ip[0] != 0 || dnsSum == 0) {
        return;
    }
    dns1 = ConvAddress(setting->ap.dns[0]);
    dns2 = ConvAddress(setting->ap.dns[1]);
    func_ov009_0215f7e8(&dns1, &dns2);
}
