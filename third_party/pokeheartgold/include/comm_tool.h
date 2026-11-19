#ifndef POKEHEARTGOLD_COMM_TOOL_H
#define POKEHEARTGOLD_COMM_TOOL_H

#include "save_layout.h"

// CommTool: the link sync-state block (sub_020379A0 allocates it into _021D414C, sizeof = 0x264 on the DS).
// Bytes only, so the layout is the same on a 64-bit host.
typedef struct UnkStruct_02036574 {
    u8 unk_00;
    u8 unk_01;
} UnkStruct_02036574;

typedef struct CommTool {
    UnkStruct_02036574 unk_00[8];
    u8 syncNo[8];
    u8 unk_18[8][70 + 2];
    u8 hasRecievedTempData[8];
    u8 syncState;
    u8 syncNoPersonal;
    u8 sendTiming;
    u8 sendTimingPending;
} CommTool;

#ifdef __MWERKS__
LAYOUT_CHECK(CommTool_size, sizeof(CommTool) == 0x264);
LAYOUT_CHECK_OFFSET(CommTool_unk_18, CommTool, unk_18, 0x18);
LAYOUT_CHECK_OFFSET(CommTool_hasRecievedTempData, CommTool, hasRecievedTempData, 0x258);
LAYOUT_CHECK_OFFSET(CommTool_syncState, CommTool, syncState, 0x260);
#endif

extern CommTool *_021D414C;

#endif // POKEHEARTGOLD_COMM_TOOL_H
