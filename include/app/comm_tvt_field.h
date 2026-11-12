// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/comm_tvt_field.h: the lines in use so
// far.
#ifndef BWPORT_APP_COMM_TVT_FIELD_H
#define BWPORT_APP_COMM_TVT_FIELD_H

#include "types.h"
#include "struct_decls.h"
#include "app/comm_tvt.h"

// The index of the friend who calls, or -1
int func_020107fc(WifiList *wifiList);

typedef struct CommTvtFieldParam CommTvtFieldParam;

// Black 2's; matching code confirms the offsets of gsys, gameData, commParam, mode and exitArgument in Black
struct CommTvtFieldParam {
    GameSystem *gsys;
    GameData *gameData;
    SaveControl *save;
    // The parameter of the video chat (overlay 257): the screen sets unk4 to 1 to call or 2 to answer, and parentMac
    CommTvtParam commParam;
    u32 unk1C;
    // 0 if the screen left to wait, 1 if it left to start the video chat
    u32 mode;
    u32 bgm;
    u32 exitArgument;
    u32 unused2C;
    u32 unused30;
};

#endif // BWPORT_APP_COMM_TVT_FIELD_H
