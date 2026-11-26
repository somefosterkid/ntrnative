#include "global.h"

#include "unk_02062108.h"
#include "unk_0205CB48.h"

void sub_0205DA1C(PlayerAvatar *avatar, LocalMapObject *object, u32 movement) {
    PlayerAvatar_SetUnk8(avatar, movement);
    MapObject_SetHeldMovement(object, movement);
}
