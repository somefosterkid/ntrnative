// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wifi_bsubway.h. The proc table's name,
// WIFI_BSUBWAY_PROC_FUNCTIONS, is pokebw2's guess.
#ifndef BWPORT_APP_WIFI_BSUBWAY_H
#define BWPORT_APP_WIFI_BSUBWAY_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"

// The Wi-Fi Battle Subway
#define OVERLAY_WIFI_BSUBWAY OVERLAY_ID(116)

extern const GameProcFunctions data_ov116_021d83a4;

// The screen's heap, a child of HEAPID_USER
#define HEAPID_WIFI_BSUBWAY 0x7d

// Black 2's; twinned code confirms the values of WIFI_BSUBWAY_RESULT_OK and WIFI_BSUBWAY_RESULT_CANCEL in Black
// How the screen ended
enum {
    WIFI_BSUBWAY_RESULT_OK,
    // An error ended it
    WIFI_BSUBWAY_RESULT_ERROR,
    // The player gave up, at the login or at a question
    WIFI_BSUBWAY_RESULT_CANCEL,
};

// Black 2's; twinned code confirms the offsets of mode and result in Black
// The proc's parameter
struct WifiBSubwayParam {
    // A WIFI_BSUBWAY_MODE_*
    u32 mode;
    GameSystem *gsys;
    // A WIFI_BSUBWAY_RESULT_*, which the proc sets
    u32 result;
};

#endif // BWPORT_APP_WIFI_BSUBWAY_H
