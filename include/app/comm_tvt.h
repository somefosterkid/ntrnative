// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/comm_tvt.h: the lines in use so far.
#ifndef BWPORT_APP_COMM_TVT_H
#define BWPORT_APP_COMM_TVT_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/overlay.h"

struct CommTvtParam {
    GameData *gameData;
    // How it starts: 0 waiting for calls, 1 calling, 2 answering the call of parentMac, 3 over the connection that
    // exists (the Wi-Fi Club's)
    u32 unk4;
    u8 parentMac[6];
};

// Black 2's, not checked against Black
// Video chat with the Xtransceiver
#define OVERLAY_COMM_TVT OVERLAY_ID(257)

#endif // BWPORT_APP_COMM_TVT_H
