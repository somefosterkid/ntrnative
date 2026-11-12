// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/demo3d.h: the lines in use so far.
#ifndef BWPORT_APP_DEMO3D_H
#define BWPORT_APP_DEMO3D_H

#include "types.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "struct_decls.h"

// The overlay with the demos (data_ov203_...), which the literal 0xcb in Black loads
#define OVERLAY_DEMO3D OVERLAY_ID(203)

// The 3D demos' heap, a child of HEAPID_USER
#define HEAPID_DEMO3D 0x6c

// What a demo is started with, which CreateSeqLoadParam fills in
struct Demo3DParam {
    GameSystem *gsys;
    u32 demoId;
    u32 unk08;
    u8 param;
    u8 hour;
    u8 minute;
    u8 season : 7;
    u8 playerSex : 1;
    u32 unk10;
    // What the demo left, 2 for one demo of the Royal Unova
    u32 result;
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
extern const GameProcFunctions data_ov203_021bfb3c;

#endif // BWPORT_APP_DEMO3D_H
