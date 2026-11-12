// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/ov306.h: the lines in use so far.
#ifndef BWPORT_APP_OV306_H
#define BWPORT_APP_OV306_H

#include "types.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
struct Ov306Param {
    GameData *gameData;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
    s32 unk14;
};

#endif // BWPORT_APP_OV306_H
