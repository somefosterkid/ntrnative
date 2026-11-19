#include "global.h"

#include "comm_player_manager.h"
#include "field_system_def.h"
#include "field_types_def.h"
#include "player_avatar.h"
#include "unk_02035900.h"

void sub_02056FD0(s32 param_1, u32 param_2, u32 param_3);

void sub_02056FD0(s32 param_1, u32 param_2, u32 param_3) {
    u32 uVar1;
    u8 uStack_18[5];

    uVar1 = PlayerAvatar_GetFacingDirection(_021D41C4->fieldSystem->playerAvatar);
    uStack_18[0] = (u8)param_2;
    uStack_18[1] = (u8)((s32)param_2 >> 8);
    uStack_18[2] = (u8)param_3;
    uStack_18[3] = (u8)((s32)param_3 >> 8);
    uStack_18[4] = (u8)uVar1;
    if (param_1 != 0) {
        uStack_18[4] = uStack_18[4] | 0x80;
    }
    sub_020376E0(0x16, &uStack_18[0]);
    _021D41C4->sendAllPositions = 1;
    return;
}
