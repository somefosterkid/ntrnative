#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "map_events_internal.h"
#include "player_avatar.h"

u8 BgEventDirectionIsCompatibleWithPlayerFacing(FieldSystem *fieldSystem, BgEvent *bgEvent);

u8 BgEventDirectionIsCompatibleWithPlayerFacing(FieldSystem *fieldSystem, BgEvent *bgEvent) {
    u32 uVar1;

    if (bgEvent->dir == 4) {
        return 1;
    }
    uVar1 = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
    switch (uVar1) {
    case 0:
        if ((bgEvent->dir == 0) || (bgEvent->dir == 6)) {
            return 1;
        }
        break;
    case 1:
        if ((bgEvent->dir == 3) || (bgEvent->dir == 6)) {
            return 1;
        }
        break;
    case 2:
        if ((bgEvent->dir == 2) || (bgEvent->dir == 5)) {
            return 1;
        }
        break;
    case 3:
        if ((bgEvent->dir == 1) || (bgEvent->dir == 5)) {
            return 1;
        }
    }
    return 0;
}
