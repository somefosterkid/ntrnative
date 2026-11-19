#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "game_stats.h"
#include "map_object.h"
#include "player_avatar.h"

void sub_0205E048(PlayerAvatar *arg0);

void sub_0205E048(PlayerAvatar *avatar) {
    FieldSystem *fieldSystem;
    GameStats *gameStats;

    fieldSystem = MapObject_GetFieldSystem(PlayerAvatar_GetMapObject(avatar));
    gameStats = Save_GameStats_Get(fieldSystem->saveData);
    GameStats_Inc(gameStats, 0);
    if (PlayerAvatar_GetState(fieldSystem->playerAvatar) == 1) {
        GameStats_Inc(gameStats, 1);
    }
}
