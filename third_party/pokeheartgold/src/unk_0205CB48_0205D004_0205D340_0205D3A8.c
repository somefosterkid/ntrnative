#include "global.h"

#include "constants/player_avatar.h"

#include "field_types_def.h"
#include "overlay_01_land_data_manager.h"

struct LandDataManager;

struct MapObject;

typedef struct MapObject MapObject;

int sub_0205D40C(PlayerAvatar *playerAvatar, int dir);

void sub_0205D494(PlayerAvatar *playerAvatar, MapObject *mapObj, int dir, u16 keyPad, u16 keyPress);

void sub_0205D4B4(PlayerAvatar *playerAvatar, MapObject *mapObj, int dir, u32 keyPad, u16 keyPress);

void sub_0205D610(PlayerAvatar *playerAvatar, MapObject *mapObj, int dir, u16 keyPad, u16 keyPress);

void sub_0205D3A8(PlayerAvatar *playerAvatar, MapObject *mapObj, const LandDataManager *landDataManager, int dir, u16 keyPad, u16 keyPress);

void sub_0205D3A8(PlayerAvatar *playerAvatar, MapObject *mapObj, const LandDataManager *landDataManager, int dir, u16 keyPad, u16 keyPress) {
    int moveState = sub_0205D40C(playerAvatar, dir);

    switch (moveState) {
    case AVATAR_MOVE_STATE_NONE:
        sub_0205D494(playerAvatar, mapObj, dir, keyPad, keyPress);
        break;
    case AVATAR_MOVE_STATE_MOVING:
        sub_0205D4B4(playerAvatar, mapObj, dir, keyPad, keyPress);
        break;
    case AVATAR_MOVE_STATE_TURNING:
        sub_0205D610(playerAvatar, mapObj, dir, keyPad, keyPress);
        break;
    }
}
