#include "global.h"

#include "constants/player_avatar.h"

#include "field_types_def.h"
#include "player_avatar.h"
#include "unk_0205CB48.h"

enum PlayerAvatarState {
    PLAYER_AVATAR_WALKING = 0,
    PLAYER_AVATAR_CYCLING,
    PLAYER_AVATAR_SURFING,

    PLAYER_AVATAR_USE_FIELD_MOVE = 16,
    PLAYER_AVATAR_SPRAYDUCK,
    PLAYER_AVATAR_CONTEST,
    PLAYER_AVATAR_FISHING,
    PLAYER_AVATAR_POKETCH,
    PLAYER_AVATAR_SAVE,
    PLAYER_AVATAR_HEAL,
    PLAYER_AVATAR_VS_SEEKER,
    PLAYER_AVATAR_DISTORTION_WORLD,
    PLAYER_AVATAR_DISTORTION_WORLD_SURF,
    PLAYER_AVATAR_26,
    PLAYER_AVATAR_27,
    PLAYER_AVATAR_HEARTHOME_GYM,
    PLAYER_AVATAR_29,
    PLAYER_AVATAR_30,
};

void sub_0205CF44(PlayerAvatar *playerAvatar) {
    PlayerAvatar_SetMoveState(playerAvatar, PLAYER_AVATAR_WALKING);
    PlayerAvatar_SetPlayerMoveState(playerAvatar, PLAYER_MOVE_STATE_NONE);
    PlayerAvatar_ClearUnk24ClearFlag2(playerAvatar);
}
