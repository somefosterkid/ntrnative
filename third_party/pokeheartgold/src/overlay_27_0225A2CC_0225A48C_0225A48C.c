#include "global.h"
struct Ov27Task;

#include "field_types_def.h"
#include "player_avatar.h"
#include "overlay_27_ov27task.h"

void ov27_0225A468(Ov27Task *, u32);

void ov27_0225A48C(Ov27Task *param_1, s32 param_2);

void ov27_0225A48C(Ov27Task *param_1, s32 param_2) {
    u16 uVar1;
    PlayerAvatar *avatar;

    if (param_2 == 0) {
        avatar = FieldSystem_GetPlayerAvatar(param_1->fieldSystem);
        uVar1 = PlayerAvatar_CheckRunningShoesLock(avatar);
        ov27_0225A468(param_1, uVar1 ^ 1);
        PlayerAvatar_SetRunningShoesLock(avatar, uVar1 ^ 1);
    }
    return;
}
