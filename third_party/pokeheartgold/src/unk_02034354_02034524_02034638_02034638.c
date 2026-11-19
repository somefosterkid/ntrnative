#include "global.h"

#include "player_data.h"
#include "unk_02034354_internal.h"
#include "unk_02035900.h"

BOOL sub_0203753C(int cmd, const void *data, int param2);

BOOL sub_02034638(void);

BOOL sub_02034638(void) {
    int netId;

    if (!_021D4130->dataRecvFlag) {
        return 0;
    }

    if (sub_0203769C() != 0) {
        return 0;
    }

    if (!sub_02037824(5)) {
        for (netId = 0; netId < 8; netId++) {
            if (_021D4130->infoState[netId] != INFO_STATE_EMPTY) {
                _021D4130->playerInfo[netId].netId = netId;
                MI_CpuCopy8(_021D4130->trainerInfo[netId], &_021D4130->playerInfo[netId].trainerInfoBuffer, PlayerProfile_sizeof());
                sub_0203753C(4, &_021D4130->playerInfo[netId], sizeof(CommPlayerInfo));
            }
        }

        sub_0203753C(5, 0L, 0);
        _021D4130->dataRecvFlag = 0;
        return 1;
    }

    return 0;
}
