#include "global.h"

#include "comm_player_manager.h"
#include "field_system_def.h"
#include "player_avatar.h"

s32 sub_02056FD0(s32, u32, u32);

void sub_0205701C(s32 arg0);

BOOL sub_02037108(int cmd, const void *data, int size);

void sub_0205701C(s32 arg0) {
    u32 v0;

    v0 = PlayerAvatar_GetXCoord(_021D41C4->fieldSystem->playerAvatar);
    sub_02056FD0(arg0, v0, PlayerAvatar_GetZCoord(_021D41C4->fieldSystem->playerAvatar));
}

void sub_0205704C(int netId, const CommPlayerPosition *playerLocation) {
    u8 data[4 + 1];
    int x = playerLocation->x, z = playerLocation->z;

    if (playerLocation->x < 0) {
        x = 0;
    } else if (playerLocation->x >= 0xf000) {
        x = 0xf000 - 1;
    }

    if (playerLocation->z < 0) {
        z = 0;
    } else if (playerLocation->z >= 0xf000) {
        z = 0xf000 - 1;
    }

    data[0] = netId & 0xf;
    data[1] = x;
    data[2] = ((x >> 8) & 0x1) + ((z >> 7) & 0x2);
    data[3] = z;

    if (playerLocation->bumped) {
        data[2] |= 0x80;
    }

    data[0] += ((playerLocation->facing % 4) << 4);
    data[0] += (playerLocation->speed << 6);

    sub_02037108(23, data, 0);
}
