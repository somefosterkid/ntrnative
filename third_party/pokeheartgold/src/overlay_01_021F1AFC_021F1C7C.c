#include "global.h"

#include "player_avatar.h"

s32 ov01_021F3084(PlayerAvatar *, u32);

void ov01_021F1C7C(PlayerAvatar *arg0);

void ov01_021F1CD0(PlayerAvatar *arg0);

void ov01_021F1CEC(PlayerAvatar *arg0);
void ov01_021F1D08(PlayerAvatar *arg0);
void ov01_021F1D24(PlayerAvatar *arg0);
void ov01_021F1D40(PlayerAvatar *arg0);

void ov01_021F1D5C(PlayerAvatar *arg0);
void ov01_021F1D78(PlayerAvatar *arg0);

s32 ov01_021F1DF4(FieldSystem *, PlayerAvatar *, s32, s32);
s32 ov01_021F2538(FieldSystem *, PlayerAvatar *, s32);
s32 sub_0205CBE4(PlayerAvatar *, s32);
s32 ov01_021F1D94(FieldSystem *fieldSystem, PlayerAvatar *playerAvatar, s32 direction, s32 playerEvents);

void ov01_021F1C7C(PlayerAvatar *avatar) {
    ov01_021F3084(avatar, PlayerAvatar_GetSpriteByStateAndGender(5, PlayerAvatar_GetGender(avatar)));
}

void ov01_021F1C98(PlayerAvatar *arg0);

void ov01_021F1C98(PlayerAvatar *avatar) {
    ov01_021F3084(avatar, PlayerAvatar_GetSpriteByStateAndGender(7, PlayerAvatar_GetGender(avatar)));
}

void ov01_021F1CB4(PlayerAvatar *arg0);

void ov01_021F1CB4(PlayerAvatar *avatar) {
    ov01_021F3084(avatar, PlayerAvatar_GetSpriteByStateAndGender(8, PlayerAvatar_GetGender(avatar)));
}

void ov01_021F1CD0(PlayerAvatar *avatar) {
    ov01_021F3084(avatar, PlayerAvatar_GetSpriteByStateAndGender(9, PlayerAvatar_GetGender(avatar)));
}

void ov01_021F1CEC(PlayerAvatar *avatar) {
    ov01_021F3084(avatar, PlayerAvatar_GetSpriteByStateAndGender(0xA, PlayerAvatar_GetGender(avatar)));
}

void ov01_021F1D08(PlayerAvatar *avatar) {
    ov01_021F3084(avatar, PlayerAvatar_GetSpriteByStateAndGender(0xB, PlayerAvatar_GetGender(avatar)));
}

void ov01_021F1D24(PlayerAvatar *avatar) {
    ov01_021F3084(avatar, PlayerAvatar_GetSpriteByStateAndGender(0xC, PlayerAvatar_GetGender(avatar)));
}

void ov01_021F1D40(PlayerAvatar *avatar) {
    ov01_021F3084(avatar, PlayerAvatar_GetSpriteByStateAndGender(6, PlayerAvatar_GetGender(avatar)));
}

void ov01_021F1D5C(PlayerAvatar *avatar) {
    ov01_021F3084(avatar, PlayerAvatar_GetSpriteByStateAndGender(0xD, PlayerAvatar_GetGender(avatar)));
}

void ov01_021F1D78(PlayerAvatar *avatar) {
    ov01_021F3084(avatar, PlayerAvatar_GetSpriteByStateAndGender(0xE, PlayerAvatar_GetGender(avatar)));
}

s32 ov01_021F1D94(FieldSystem *fieldSystem, PlayerAvatar *playerAvatar, s32 direction, s32 playerEvents) {
    if (PlayerAvatar_CheckForcedMovement(playerAvatar) == 1) {
        if (ov01_021F2538(fieldSystem, playerAvatar, playerEvents) == 1) {
            return 1;
        }
        return 0;
    }
    if (sub_0205CBE4(playerAvatar, direction) == 0) {
        return 0;
    }
    if (direction == -1) {
        return 0;
    }
    if (ov01_021F1DF4(fieldSystem, playerAvatar, direction, playerEvents) == 1) {
        return 1;
    }
    return 0;
}
