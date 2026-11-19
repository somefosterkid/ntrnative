#include "global.h"

#include "constants/player_avatar.h"

#include "field_types_def.h"

struct MapObject;
typedef struct MapObject MapObject;

enum AvatarMoveState sub_0205D428(enum AvatarMoveState moveState);
int sub_0205D44C(MapObject *mapObj, u8 tileBehavior, int movementAction);

enum AvatarMoveState sub_0205D428(enum AvatarMoveState moveState) {
    switch (moveState) {
    case AVATAR_MOVE_STATE_NONE:
        return AVATAR_MOVE_STATE_NONE;
    case AVATAR_MOVE_STATE_MOVING:
        return AVATAR_MOVE_STATE_MOVING;
    case AVATAR_MOVE_STATE_TURNING:
        return AVATAR_MOVE_STATE_TURNING;
    }

    ((0) ? (void)0 : GF_AssertFail());
    return AVATAR_MOVE_STATE_NONE;
}

int sub_0205D44C(MapObject *mapObj, u8 tileBehavior, int movementAction) {
    return movementAction;
}
