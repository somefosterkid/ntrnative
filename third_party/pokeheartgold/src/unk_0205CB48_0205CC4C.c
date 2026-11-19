#include "global.h"

#include "constants/player_avatar.h"

#include "field_types_def.h"
#include "player_avatar.h"

enum FaceDirection {
    FACE_NONE = -1,
    FACE_UP = 0,
    FACE_DOWN,
    FACE_LEFT,
    FACE_RIGHT,
};

enum FaceDirection sub_0205DD9C(u16 pad);

enum FaceDirection sub_0205DDB8(u16 pad);

void sub_0205CC4C(PlayerAvatar *playerAvatar, int dir, u16 keyPad, u16 keyPress);

void sub_0205CC74(PlayerAvatar *arg0);

void sub_0205CC4C(PlayerAvatar *playerAvatar, int dir, u16 keyPad, u16 keyPress) {
    PlayerAvatar_SetUnk28Unk2C(playerAvatar, sub_0205DD9C(keyPress), sub_0205DDB8(keyPress));
    PlayerAvatar_ClearFlag6(playerAvatar);
}

void sub_0205CC74(PlayerAvatar *avatar) {
    if ((PlayerAvatar_CheckFlag6(avatar) == 1) && (PlayerAvatar_GetMoveState(avatar) == 1)) {
        PlayerAvatar_SetFlag1(avatar, 0);
    }
}
