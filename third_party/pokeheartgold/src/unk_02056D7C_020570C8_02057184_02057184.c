#include "global.h"

#include "comm_player_manager.h"
#include "field_system_def.h"
#include "error_handling.h"
#include "map_object.h"
#include "player_avatar.h"
#include "player_data.h"
#include "unk_02034354.h"
#include "unk_02035900.h"

void sub_02057184(u32 arg0);

void sub_02057184(u32 arg0) {
    PlayerProfile *profile;
    LocalMapObject *obj;
    PlayerAvatar *avatar;
    u32 version;
    u8 gameCode;

    if (_021D41C4->avatars[arg0] != NULL) {
        return;
    }
    if (_021D41C4->unk_EE) {
        return;
    }
    profile = sub_02034818(arg0);
    if (profile != NULL) {
        if (arg0 != sub_0203769C()) {
            obj = MapObjectManager_GetFirstActiveObjectByID(_021D41C4->fieldSystem->mapObjectManager, arg0 + 0x100);
            if (obj != NULL) {
                MapObject_Remove(obj);
            }
        }
        version = 2;
        gameCode = PlayerProfile_GetVersion(profile);
        if (gameCode == 0) {
            version = 1;
        } else if (gameCode == 0xC) {
            version = 0;
        }

        avatar = PlayerAvatar_CreateWithParams(_021D41C4->fieldSystem->mapObjectManager, _021D41C4->positions[arg0].x, _021D41C4->positions[arg0].z, _021D41C4->positions[arg0].facing, 0, PlayerProfile_GetTrainerGender(profile), version, NULL);
        GF_ASSERT(avatar != NULL);
        _021D41C4->avatars[arg0] = avatar;
        MapObject_SetID(PlayerAvatar_GetMapObject(avatar), arg0 + 0x100);
        _021D41C4->active[arg0] = 1;
    }
}
