#include "global.h"

#include "comm_player_manager.h"
#include "unk_0205FD20.h"

u16 sub_02057B4C(s32 arg0);

s32 sub_02057BEC(s32 arg0);

void sub_02057C5C(s32 param_1, u32 param_2);

s32 sub_02057BEC(s32 arg0) {

    if (sub_02057B4C(arg0) == 0xFFFF) {
        return 0xFFFF;
    }
    return _021D41C4->hostPositions[arg0].x + GetDeltaXByFacingDirection((s32)_021D41C4->hostPositions[arg0].facing);
}

u16 sub_02057B9C(s32 arg0);

s32 sub_02057C24(s32 arg0);

s32 sub_02057C24(s32 arg0) {

    if (sub_02057B9C(arg0) == 0xFFFF) {
        return 0xFFFF;
    }
    return _021D41C4->hostPositions[arg0].z + GetDeltaYByFacingDirection((s32)_021D41C4->hostPositions[arg0].facing);
}

void sub_02057C5C(s32 param_1, u32 param_2) {
    if (param_2 != _021D41C4->moveEnabled[param_1]) {
        _021D41C4->moveEnabled[param_1] = (s8)param_2;
        _021D41C4->hostPositions[param_1].bumped = 0;
        _021D41C4->positionChanged[param_1] = 1;
        _021D41C4->hostPositions[param_1].speed = 2;
    }
    return;
}
