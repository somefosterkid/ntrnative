#include "global.h"

#include "constants/player_avatar.h"

#include "field_types_def.h"

enum AvatarBikeState {
    AVATAR_BIKE_STATE_NONE = 0,
    AVATAR_BIKE_STATE_MOVING,
    AVATAR_BIKE_STATE_TURNING,
    AVATAR_BIKE_STATE_BRAKE,
};

int sub_0205D778(int bikeState);

int sub_0205D778(int bikeState) {
    switch (bikeState) {
    case AVATAR_BIKE_STATE_NONE:
        return AVATAR_MOVE_STATE_NONE;
    case AVATAR_BIKE_STATE_MOVING:
        return AVATAR_MOVE_STATE_MOVING;
    case AVATAR_BIKE_STATE_TURNING:
        return AVATAR_MOVE_STATE_TURNING;
    case AVATAR_BIKE_STATE_BRAKE:
        return AVATAR_MOVE_STATE_MOVING;
    }

    ((0) ? (void)0 : GF_AssertFail());
    return AVATAR_MOVE_STATE_NONE;
}
