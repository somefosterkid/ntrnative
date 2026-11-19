#include "global.h"

#include "comm_player_manager.h"
#include "field_system_def.h"
#include "player_avatar.h"
#include "unk_02035900.h"

s32 sub_0205701C(s32);

void sub_020570C8(FieldSystem *unused) {
    u32 v0;
    u32 v1;

    v0 = PlayerAvatar_GetMoveState(_021D41C4->fieldSystem->playerAvatar);
    v1 = PlayerAvatar_GetPlayerMoveState(_021D41C4->fieldSystem->playerAvatar);
    if ((v0 == 0) && (sub_020373B4(sub_0203769C()) != 0) && (_021D41C4->localPositionSent == 0)) {
        sub_0205701C(1);
        _021D41C4->localPositionSent = 1U;
    }
    if ((v0 == 1) && (v1 == 1)) {
        _021D41C4->localPositionSent = 0U;
    }
}
