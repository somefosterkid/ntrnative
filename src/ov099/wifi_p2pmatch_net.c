// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov173/wifi_p2pmatch_net.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/wifi_p2pmatch_net.h"
#include "app/wifi_p2pmatch.h"
#include "gfl/net_command.h"
#include "struct_decls.h"

// The command table of the Wi-Fi Club's match board. The ROM has no name for this file, and the name is a guess; the
// handlers that do something are in wifi_p2pmatch.c

enum {
    WIFIP2PMATCH_NET_CMD_UNUSED0,
    WIFIP2PMATCH_NET_CMD_UNUSED1,
    WIFIP2PMATCH_NET_CMD_UNUSED2,
    WIFIP2PMATCH_NET_CMD_3,
    WIFIP2PMATCH_NET_CMD_4,
    WIFIP2PMATCH_NET_CMD_5,
    WIFIP2PMATCH_NET_CMD_6,
    WIFIP2PMATCH_NET_CMD_7,
    WIFIP2PMATCH_NET_CMD_8,
    WIFIP2PMATCH_NET_CMD_NUM,
};

static const NetCommand *func_ov099_021ba7cc(void);
static int func_ov099_021ba7d4(void);
static void func_ov099_021ba804(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov099_021ba808(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov099_021ba80c(int netId, int size, const void *data, void *work, NetHandle *handle);

// pokebw2's sWifiP2PMatchNetCommands
static const NetCommand data_ov099_021c28a8[WIFIP2PMATCH_NET_CMD_NUM] = {
    { func_ov099_021ba804, NULL },
    { func_ov099_021ba808, NULL },
    { func_ov099_021ba80c, NULL },
    { func_ov099_021c1350, NULL },
    { func_ov099_021c1288, NULL },
    { func_ov099_021c12fc, NULL },
    { func_ov099_021c1334, NULL },
    { func_ov099_021c12cc, func_ov099_021c12ec },
    { func_ov099_021c12d0, NULL },
};

// pokebw2's WifiP2PMatchNet_CommandTableGet
static const NetCommand *func_ov099_021ba7cc(void) {
    return data_ov099_021c28a8;
}

// pokebw2's WifiP2PMatchNet_CommandNumGet
static int func_ov099_021ba7d4(void) {
    return WIFIP2PMATCH_NET_CMD_NUM;
}

// pokebw2's WifiP2PMatchNet_CommandInit
void func_ov099_021ba7d8(void *work) {
    const NetCommand *commands = func_ov099_021ba7cc();
    int num = func_ov099_021ba7d4();

    func_0203a574(WIFIP2PMATCH_NET_CMD_BASE, commands, num, work);
}

// pokebw2's WifiP2PMatchNet_CommandExit
void func_ov099_021ba7f8(void *work) {
    func_0203a5e4(WIFIP2PMATCH_NET_CMD_BASE);
}

// pokebw2's WifiP2PMatchNet_RecvUnused0
static void func_ov099_021ba804(int netId, int size, const void *data, void *work, NetHandle *handle) {
    if (work == NULL) {
        return;
    }
}

// pokebw2's WifiP2PMatchNet_RecvUnused1
static void func_ov099_021ba808(int netId, int size, const void *data, void *work, NetHandle *handle) {
    if (work == NULL) {
        return;
    }
}

// pokebw2's WifiP2PMatchNet_RecvUnused2
static void func_ov099_021ba80c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    if (work == NULL) {
        return;
    }
}
