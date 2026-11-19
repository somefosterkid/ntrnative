#include "global.h"

#include "comm_player_manager.h"
#include "field_system_def.h"
#include "field_types_def.h"
#include "player_avatar.h"
#include "system.h"

void sub_0205724C(s32 param_1, s32 param_2, s32 param_3);

s32 sub_02037498(s32);
void sub_020572AC(FieldSystem *arg0);

void sub_0205724C(s32 param_1, s32 param_2, s32 param_3) {
    PlayerAvatar *avatar;

    if (_021D41C4 != 0) {
        avatar = _021D41C4->avatars[param_1];
        if (avatar != NULL) {
            if (_021D41C4->fieldSystem->playerAvatar != avatar) {
                if (param_3 != 0) {
                    PlayerAvatar_DeleteFromMap(avatar);
                } else {
                    PlayerAvatar_FreeToHeap(avatar);
                }
            }
            _021D41C4->avatars[param_1] = NULL;
        }
        if ((_021D41C4->active[param_1] != 0) && (param_2 == 0)) {
            _021D41C4->active[param_1] = 0;
        }
        _021D41C4->positionChanged[param_1] = 1;
    }
    return;
}

void sub_020572AC(FieldSystem *fieldSystem) {
    s32 v0;

    v0 = 2;
    if (gSystem.heldKeys & 2) {
        v0 = 1;
    } else if ((fieldSystem != NULL) && (PlayerAvatar_CheckRunningShoesLock(FieldSystem_GetPlayerAvatar(fieldSystem)) != 0)) {
        v0 = 1;
    }
    sub_02037498(v0);
}
