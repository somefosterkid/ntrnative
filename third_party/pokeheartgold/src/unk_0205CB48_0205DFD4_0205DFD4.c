#include "global.h"

#include "unk_0205CB48.h"

#include "player_avatar.h"
#include "pokemon_types_def.h"


void sub_0205DFD4(PlayerAvatar *arg0, s32 arg1);

void sub_0205DFD4(PlayerAvatar *avatar, s32 arg1) {
    sub_0205DA1C(avatar, PlayerAvatar_GetMapObject(avatar), arg1);
}
