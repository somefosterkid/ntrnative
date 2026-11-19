#include "global.h"

#include "comm_player_manager.h"
#include "field_system_def.h"
#include "player_avatar.h"

s32 sub_02056EF4(void);

s32 sub_0205701C(s32);

void sub_02056E60(void);

void sub_02056E60(void) {
    if (_021D41C4 != NULL) {
        _021D41C4->sendAllPositions = 1;
        _021D41C4->unk_EE = 0;
        sub_02056EF4();
        sub_0205701C(1);
        PlayerAvatar_GetXCoord(_021D41C4->fieldSystem->playerAvatar);
        PlayerAvatar_GetZCoord(_021D41C4->fieldSystem->playerAvatar);
    }
}
