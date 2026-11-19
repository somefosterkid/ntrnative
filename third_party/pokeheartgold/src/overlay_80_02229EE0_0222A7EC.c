#include "global.h"

#include "overlay_80_02229EE0.h"
#include "player_data.h"
#include "constants/player_avatar.h"
#include "field_types_def.h"


struct MapObject;
typedef struct MapObject MapObject;
MapObject *Save_WiFiHistory_Get(PlayerAvatar *playerAvatar);
BOOL sub_02039F68(const MapObject *mapObj);

s32 ov80_0222A7EC(PlayerProfile *profile) {
    s32 v0;
    u32 v1;

    v1 = PlayerProfile_GetTrainerGender(profile);
    v0 = (s32)PlayerProfile_GetVersion(profile);
    switch (v0) {
    default:
    case 7:
    case 8:
        if (v1 == 0) {
            return 0;
        }
        return 0x61;
    case 0:
        if (v1 == 0) {
            return 0xEE;
        }
        return 0xEF;
    case 12:
        if (v1 == 0) {
            return 0x127;
        }
        return 0x128;
    }
}

BOOL ov80_0222A840(PlayerAvatar *playerAvatar) {
    MapObject *mapObj = Save_WiFiHistory_Get(playerAvatar);
    return sub_02039F68(mapObj);
}
