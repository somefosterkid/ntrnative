#include "global.h"

#include "comm_player_manager.h"
#include "unk_02035900.h"
#include "unk_02056D7C.h"

u16 sub_02057A34(s32 arg0);

u16 sub_02057A34(s32 arg0) {
    if (_021D41C4 == 0) {
        return 0xFFFFU;
    }
    if (arg0 == sub_0203769C()) {
        return _021D41C4->positions[arg0].x;
    }
    if ((sub_02057A0C() == 0) || (_021D41C4->active[arg0] == 0)) {
        return 0xFFFFU;
    }
    return _021D41C4->positions[arg0].x;
}
