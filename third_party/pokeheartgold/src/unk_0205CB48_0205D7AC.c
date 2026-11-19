#include "global.h"

#include "constants/player_avatar.h"

#include "player_avatar.h"
#include "map_object.h"

enum AvatarBikeState {
    AVATAR_BIKE_STATE_NONE = 0,
    AVATAR_BIKE_STATE_MOVING,
    AVATAR_BIKE_STATE_TURNING,
    AVATAR_BIKE_STATE_BRAKE,
};

enum AvatarMoveSpeed {
    AVATAR_MOVE_SPEED_0 = 0,
    AVATAR_MOVE_SPEED_1,
    AVATAR_MOVE_SPEED_2,
    AVATAR_MOVE_SPEED_3,

    AVATAR_MOVE_SPEED_BIKE_BRAKE = AVATAR_MOVE_SPEED_2,
};

enum AvatarBikeState sub_0205D7AC(PlayerAvatar *playerAvatar, int dir);

s32 sub_0205DA1C(PlayerAvatar *, LocalMapObject *, s32);
s32 sub_0206234C(u32, s32);
void sub_0205D818(PlayerAvatar *arg0, LocalMapObject *arg1, int dir, u16 keyPad, u16 keyPress);

enum AvatarBikeState sub_0205D7AC(PlayerAvatar *playerAvatar, int dir) {
    int speed = PlayerAvatar_GetUnk24(playerAvatar);

    if (dir == DIR_NONE) {
        if (speed < AVATAR_MOVE_SPEED_BIKE_BRAKE) {
            PlayerAvatar_SetMoveState(playerAvatar, AVATAR_MOVE_STATE_NONE);
            return AVATAR_BIKE_STATE_NONE;
        }

        PlayerAvatar_SetMoveState(playerAvatar, AVATAR_MOVE_STATE_MOVING);
        return AVATAR_BIKE_STATE_BRAKE;
    }

    int facingDir = PlayerAvatar_GetFacingDirection(playerAvatar);

    if (facingDir != dir && PlayerAvatar_GetMoveState(playerAvatar) != AVATAR_MOVE_STATE_MOVING) {
        if (speed < AVATAR_MOVE_SPEED_BIKE_BRAKE) {
            PlayerAvatar_SetMoveState(playerAvatar, AVATAR_MOVE_STATE_TURNING);
            return AVATAR_BIKE_STATE_TURNING;
        }

        PlayerAvatar_SetMoveState(playerAvatar, AVATAR_MOVE_STATE_MOVING);
        return AVATAR_BIKE_STATE_BRAKE;
    }

    PlayerAvatar_SetMoveState(playerAvatar, AVATAR_MOVE_STATE_MOVING);
    return AVATAR_BIKE_STATE_MOVING;
}

void sub_0205D818(PlayerAvatar *avatar, LocalMapObject *object, int dir, u16 keyPad, u16 keyPress) {
    sub_0205DA1C(avatar, object, sub_0206234C(MapObject_GetFacingDirection(object), 0));
    PlayerAvatar_ClearUnk24ClearFlag2(avatar);
}
