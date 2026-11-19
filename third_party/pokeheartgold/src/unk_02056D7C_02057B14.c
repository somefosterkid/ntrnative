#include "global.h"

#include "comm_player_manager.h"
#include "unk_02035900.h"
#include "unk_02056D7C.h"
#include "unk_0205FD20.h"

u16 sub_02057A88(s32 arg0);

s32 sub_02057B14(s32 arg0);

u16 sub_02057B9C(s32 arg0);

s32 sub_02057B14(s32 arg0) {

    if (sub_02057A88(arg0) == 0xFFFF) {
        return 0xFFFF;
    }
    return _021D41C4->positions[arg0].z + GetDeltaYByFacingDirection((s32)_021D41C4->positions[arg0].facing);
}

u16 sub_02057B4C(s32 arg0);

u16 sub_02057B4C(s32 arg0) {
    if (_021D41C4 == 0) {
        return 0xFFFFU;
    }
    if (arg0 == sub_0203769C()) {
        return _021D41C4->hostPositions[arg0].x;
    }
    if ((sub_02057A0C() == 0) || (_021D41C4->active[arg0] == 0)) {
        return 0xFFFFU;
    }
    return _021D41C4->hostPositions[arg0].x;
}

u16 sub_02057B9C(s32 arg0) {
    if (_021D41C4 == 0) {
        return 0xFFFFU;
    }
    if (arg0 == sub_0203769C()) {
        return _021D41C4->hostPositions[arg0].z;
    }
    if ((sub_02057A0C() == 0) || (_021D41C4->active[arg0] == 0)) {
        return 0xFFFFU;
    }
    return _021D41C4->hostPositions[arg0].z;
}
