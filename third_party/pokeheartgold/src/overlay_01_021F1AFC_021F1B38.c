#include "global.h"

#include "overlay_01_021F1348.h"
#include "player_avatar.h"

s32 ov01_021F3084(PlayerAvatar *, u32);

void ov01_021F1B38(PlayerAvatar *arg0);

void ov01_021F1B38(PlayerAvatar *avatar) {
    OverworldAnimManager *a0;

    ov01_021F3084(avatar, PlayerAvatar_GetSpriteByStateAndGender(0, PlayerAvatar_GetGender(avatar)));
    PlayerAvatar_SetState(avatar, 0);
    PlayerAvatar_ClearUnk24ClearFlag2(avatar);
    a0 = PlayerAvatar_GetUnk34(avatar);
    if (a0 != 0) {
        ov01_021F1640(a0);
    }
    PlayerAvatar_SetUnk34(avatar, NULL);
}
