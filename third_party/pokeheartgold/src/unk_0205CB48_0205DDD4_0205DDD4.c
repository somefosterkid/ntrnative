#include "global.h"

#include "field_types_def.h"
#include "player_avatar.h"

enum FaceDirection {
    FACE_NONE = -1,
    FACE_UP = 0,
    FACE_DOWN,
    FACE_LEFT,
    FACE_RIGHT,
};

enum FaceDirection sub_0205DD9C(u16);

enum FaceDirection sub_0205DDB8(u16);

enum FaceDirection sub_0205DDD4(PlayerAvatar *playerAvatar, u16 pressedKeys, u16 heldKeys);

enum FaceDirection sub_0205DDD4(PlayerAvatar *playerAvatar, u16 pressedKeys, u16 heldKeys) {
    s32 v0;
    enum FaceDirection v2;
    enum FaceDirection v4;
    s32 v1;
    u32 v3;

    v2 = sub_0205DD9C(heldKeys);
    v4 = sub_0205DDB8(heldKeys);
    if (v2 != FACE_NONE) {
        if (v4 == FACE_NONE) {
            return v2;
        }
        v3 = PlayerAvatar_GetNextFacingDirection(playerAvatar);
        v0 = PlayerAvatar_GetUnk28(playerAvatar);
        v1 = PlayerAvatar_GetUnk2C(playerAvatar);
        if (v3 != -1U) {
            if ((v2 == v0) && (v4 == v1)) {
                return (enum FaceDirection)v3;
            }
            if (v4 != v1) {
                return v4;
            }
            return v2;
        }
        return v4;
    }
    return v4;
}
