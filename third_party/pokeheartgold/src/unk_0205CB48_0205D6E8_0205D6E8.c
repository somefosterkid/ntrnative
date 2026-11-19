#include "global.h"

#include "constants/player_avatar.h"

#include "field_types_def.h"
#include "overlay_01_land_data_manager.h"

struct LandDataManager;

struct MapObject;

enum AvatarBikeState {
    AVATAR_BIKE_STATE_NONE = 0,
    AVATAR_BIKE_STATE_MOVING,
    AVATAR_BIKE_STATE_TURNING,
    AVATAR_BIKE_STATE_BRAKE,
};

typedef struct MapObject MapObject;

enum AvatarBikeState sub_0205D75C(PlayerAvatar *playerAvatar, int dir);

void sub_0205D818(PlayerAvatar *playerAvatar, MapObject *mapObj, int dir, u16 keyPad, u16 keyPress);

void sub_0205D83C(PlayerAvatar *playerAvatar, MapObject *mapObj, int dir, u16 keyPad, u16 keyPress);

void sub_0205D948(PlayerAvatar *playerAvatar, MapObject *mapObj, int dir, u16 keyPad, u16 keyPress);

void sub_0205D978(PlayerAvatar *playerAvatar, MapObject *mapObj, int dir, u16 keyPad, u16 keyPress);

void sub_0205D6E8(PlayerAvatar *playerAvatar, MapObject *mapObj, const LandDataManager *landDataManager, int dir, u16 keyPad, u16 keyPress);

void sub_0205D6E8(PlayerAvatar *playerAvatar, MapObject *mapObj, const LandDataManager *landDataManager, int dir, u16 keyPad, u16 keyPress) {
    enum AvatarBikeState bikeState = sub_0205D75C(playerAvatar, dir);

    switch (bikeState) {
    case AVATAR_BIKE_STATE_NONE:
        sub_0205D818(playerAvatar, mapObj, dir, keyPad, keyPress);
        break;
    case AVATAR_BIKE_STATE_MOVING:
        sub_0205D83C(playerAvatar, mapObj, dir, keyPad, keyPress);
        break;
    case AVATAR_BIKE_STATE_TURNING:
        sub_0205D948(playerAvatar, mapObj, dir, keyPad, keyPress);
        break;
    case AVATAR_BIKE_STATE_BRAKE:
        sub_0205D978(playerAvatar, mapObj, dir, keyPad, keyPress);
        break;
    }
}
