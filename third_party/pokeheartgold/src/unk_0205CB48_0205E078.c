#include "global.h"

#include "constants/player_avatar.h"

#include "field_types_def.h"

struct MapObject;
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
typedef struct MapObject MapObject;
MapObject *PlayerAvatar_GetMapObject(PlayerAvatar *playerAvatar);
int PlayerAvatar_GetState(PlayerAvatar *playerAvatar);
BOOL PlayerAvatar_CheckBikeStateLocked(PlayerAvatar *playerAvatar);
int sub_02060EEC(MapObject *mapObj, u32 tileBehavior);

BOOL sub_0205E078(PlayerAvatar *playerAvatar, u32 tileBehavior, int dir);

BOOL sub_0205E078(PlayerAvatar *playerAvatar, u32 tileBehavior, int dir) {
    if (dir != -1) {
        return 0;
    }

    if (PlayerAvatar_GetState(playerAvatar) != PLAYER_AVATAR_CYCLING) {
        return 0;
    }

    if (!PlayerAvatar_CheckBikeStateLocked(playerAvatar)) {
        return 0;
    }

    MapObject *mapObj = PlayerAvatar_GetMapObject(playerAvatar);

    if (sub_02060EEC(mapObj, tileBehavior) == 1) {
        return 1;
    }

    return 0;
}
