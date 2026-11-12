// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/net_define.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "system/net_define.h"
#include "field/net_devirc.h"
#include "gfl/net.h"
#include "gfl/net_devwifi.h"
#include "gfl/net_devwireless.h"
#include "gfl/overlay.h"
#include "gfl/wm_icon.h"

// What the game gives the network library: the Wi-Fi icon's files, the device of each kind of connection with the
// overlays it needs, the network's step of each frame and the kind of the game commands. The ROM has no string for
// the file, so the name is a guess. getWifiIconNarcIdxNum is swan's name.

// The field's overlays 10 and 11 make room for the Wi-Fi library's 9 and 13, and the GTS's 114
#define OVERLAY_NET_WIRELESS OVERLAY_ID(14)
#define OVERLAY_NET_IRC OVERLAY_ID(15)

// pokebw2's sSkipFrame
static u8 data_02146178;

u32 getWifiIconNarcIdxNum(void) {
    return 0;
}

void func_0200f16c(WmIconFiles *files) {
    files->chars[0] = 2;
    files->chars[1] = 4;
    files->pltt = 1;
}

const GFLNetDevTable *func_0200f17c(int type) {
    switch (type) {
    case GFL_NET_TYPE_WIFI_GTS:
        GFL_OvlUnload(OVERLAY_ID(10));
        GFL_OvlUnload(OVERLAY_ID(11));
        GFL_OvlLoad(OVERLAY_ID(9));
        GFL_OvlLoad(OVERLAY_ID(13));
        GFL_OvlLoad(OVERLAY_ID(114));
        return func_ov009_0215536c();
    case GFL_NET_TYPE_WIFI:
        GFL_OvlUnload(OVERLAY_ID(10));
        GFL_OvlUnload(OVERLAY_ID(11));
        GFL_OvlLoad(OVERLAY_ID(9));
        GFL_OvlLoad(OVERLAY_ID(13));
        return func_ov009_0215536c();
    case GFL_NET_TYPE_WIFI_LOBBY:
        GFL_OvlUnload(OVERLAY_ID(10));
        GFL_OvlUnload(OVERLAY_ID(11));
        GFL_OvlLoad(OVERLAY_ID(9));
        GFL_OvlLoad(OVERLAY_ID(13));
        return func_ov009_0215536c();
    case 0:
    case 5:
        GFL_OvlLoad(OVERLAY_NET_WIRELESS);
        return func_ov014_02177020();
    case 3:
    case 4:
        GFL_OvlLoad(OVERLAY_NET_IRC);
        return func_ov015_021798d4();
    }
    return NULL;
}


void func_0200f234(int type) {
    switch (type) {
    case GFL_NET_TYPE_WIFI_GTS:
        GFL_OvlUnload(OVERLAY_ID(114));
        GFL_OvlUnload(OVERLAY_ID(9));
        GFL_OvlUnload(OVERLAY_ID(13));
        GFL_OvlLoad(OVERLAY_ID(10));
        GFL_OvlLoad(OVERLAY_ID(11));
        break;
    case GFL_NET_TYPE_WIFI:
        GFL_OvlUnload(OVERLAY_ID(9));
        GFL_OvlUnload(OVERLAY_ID(13));
        GFL_OvlLoad(OVERLAY_ID(10));
        GFL_OvlLoad(OVERLAY_ID(11));
        break;
    case GFL_NET_TYPE_WIFI_LOBBY:
        GFL_OvlUnload(OVERLAY_ID(9));
        GFL_OvlUnload(OVERLAY_ID(13));
        GFL_OvlLoad(OVERLAY_ID(10));
        GFL_OvlLoad(OVERLAY_ID(11));
        break;
    case 0:
    case 5:
        GFL_OvlUnload(OVERLAY_NET_WIRELESS);
        break;
    case 3:
    case 4:
        GFL_OvlUnload(OVERLAY_NET_IRC);
        break;
    }
}


void func_0200f2d4(void) {
    if (func_0203d5d4() == 0x2) {
        if (!data_02146178) {
            data_02146178 = func_0203cf5c();
        } else {
            data_02146178 = FALSE;
        }
    } else {
        func_0203cf5c();
    }
}

BOOL func_0200f300(void) {
    switch (func_0203d5d4()) {
    case 0xc:
    case 0xd:
    case 0xe:
    case 0xf:
    case 0x10:
    case 0x1c:
    case 0x1f:
    case 0x25:
    case 0x26:
    case 0x30:
        return TRUE;
    }
    return FALSE;
}
