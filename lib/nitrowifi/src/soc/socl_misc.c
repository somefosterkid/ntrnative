// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/soc/socl_misc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "nitrowifi/soc.h"
#include "socl_internal.h"
#include "gfl/std.h"

// SOCL's helpers: the rounding of sizes, the socket counters and the checks of the address configuration that CPS asks
// for. pokediamond's socl_misc.o has only func_ov009_0215f0c8 (SOCLi_RoundUp4); pokebw2's socl_misc.c and Black 1's
// decompilation (aletheia) have the same five functions, with the SDK's names from TwlSDK's libsoc (socl_misc.o), and
// pokebw2's ov011 delinks.txt puts SOCLiSocketCount in this file's .bss, before socl_list.c's

static BOOL func_ov009_0215f184(u32 net_mask, u32 ip);
static BOOL func_ov009_0215f0f0(u32 net_mask, u32 ip, u32 my_ip);

// pokebw2's SOCLiSocketCount
s32 data_ov009_021aa4ac[2];

// pokebw2's SOCLi_RoundUp4
u32 func_ov009_0215f0c8(u32 value) {
    return (value + 3) & ~3;
}

// pokebw2's SOCLi_InitResource
void func_ov009_0215f0d4(void) {
    MI_CpuFill8(data_ov009_021aa4ac, 0, sizeof(data_ov009_021aa4ac));
}

// Whether ip can be a remote address for a host with this net mask and address: not the network or broadcast address
// of the host's own subnet, not 0, not 255.255.255.255, not loopback, not multicast
// pokebw2's SOCLi_IsIPv4ValidDNSAddr
static BOOL func_ov009_0215f0f0(u32 net_mask, u32 ip, u32 my_ip) {
    BOOL not_multicast = FALSE;
    BOOL not_loopback = FALSE;
    BOOL ok = FALSE;

    if (ip != 0xffffffff && ip != 0 && my_ip != ip) {
        if (((my_ip ^ ip) & net_mask) != 0 || (ip & ~net_mask) != 0) {
            BOOL unicast = TRUE;

            /* FAKEMATCH(cast): the casts only stop MWCC reusing the first (my_ip ^ ip) & net_mask; the original
               probably got the recompute from an SDK inline or macro; clean up later. Round 37: (ip ^ my_ip) in
               the second test 14 lines, (my_ip & net_mask) == (ip & net_mask) 4, pokebw2's socl_misc.c form 18,
               the SDK's unsigned long for all or some parameters 14 (pokebw2 and Black 1's aletheia name it
               SOCLi_IsIPv4ValidDNSAddr, TwlSDK libsoc's socl_misc.o) */
            if ((((u32)my_ip ^ ip) & (u32)net_mask) == 0 && ((ip & ~net_mask) ^ ~net_mask) == 0) {
                unicast = FALSE;
            }
            if (unicast) {
                ok = TRUE;
            }
        }
    }
    if (ok) {
        if (((ip & 0xff000000) ^ 0x7f000000) != 0) {
            not_loopback = TRUE;
        }
    }
    if (not_loopback) {
        if (((ip & 0xf0000000) ^ 0xe0000000) != 0) {
            not_multicast = TRUE;
        }
    }
    return not_multicast;
}

// Whether ip can be the host's own address for this net mask: not the subnet's network or broadcast address, not
// loopback, not multicast
// pokebw2's SOCLi_IsIPv4ValidNodeAddr
static BOOL func_ov009_0215f184(u32 net_mask, u32 ip) {
    BOOL not_multicast = FALSE;
    BOOL not_loopback = FALSE;
    BOOL host_ok = FALSE;
    u32 host = ip & ~net_mask;

    if ((~net_mask ^ host) != 0 && host != 0) {
        host_ok = TRUE;
    }
    if (host_ok) {
        if (((ip & 0xff000000) ^ 0x7f000000) != 0) {
            not_loopback = TRUE;
        }
    }
    if (not_loopback) {
        if (((ip & 0xf0000000) ^ 0xe0000000) != 0) {
            not_multicast = TRUE;
        }
    }
    return not_multicast;
}

// pokebw2's SOCLi_CheckNConfig
BOOL func_ov009_0215f1d4(void) {
    if (data_ov009_021a827c == 0 || ((~data_ov009_021a827c + 1) & ~data_ov009_021a827c) != 0) {
        return FALSE;
    }
    if (!func_ov009_0215f184(data_ov009_021a827c, data_ov009_021a82bc)) {
        return FALSE;
    }
    if (data_ov009_021a8298 != 0) {
        if (!func_ov009_0215f184(data_ov009_021a827c, data_ov009_021a8298) ||
            (data_ov009_021a827c != 0xffffffff && (data_ov009_021a82bc & data_ov009_021a827c) != (data_ov009_021a8298 & data_ov009_021a827c)) ||
            data_ov009_021a82bc == data_ov009_021a8298) {
            data_ov009_021a8298 = 0;
        }
    }
    if (!func_ov009_0215f0f0(data_ov009_021a827c, data_ov009_021a82dc[0], data_ov009_021a82bc)) {
        data_ov009_021a82dc[0] = 0;
    }
    if (!func_ov009_0215f0f0(data_ov009_021a827c, data_ov009_021a82dc[1], data_ov009_021a82bc)) {
        data_ov009_021a82dc[1] = 0;
    }
    return TRUE;
}
