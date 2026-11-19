#include "global.h"

#include "field_types_def.h"
#include "overlay_01_021F1348.h"
#include "player_avatar.h"
#include "unk_02005D10.h"

void ov01_021F3084(PlayerAvatar *, u32);

void ov01_021F1B78(PlayerAvatar *param_1);

void ov01_021F1B78(PlayerAvatar *param_1) {
    u32 uVar1;
    OverworldAnimManager *a0;

    uVar1 = PlayerAvatar_GetGender(param_1);
    uVar1 = PlayerAvatar_GetSpriteByStateAndGender(1, uVar1);
    ov01_021F3084(param_1, uVar1);
    PlayerAvatar_SetState(param_1, 1);
    PlayerAvatar_ClearUnk24ClearFlag2(param_1);
    a0 = PlayerAvatar_GetUnk34(param_1);
    if (a0 != 0) {
        ov01_021F1640(a0);
    }
    PlayerAvatar_SetUnk34(param_1, 0);
    PlaySE(0x617);
    return;
}
