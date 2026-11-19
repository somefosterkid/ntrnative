#include "global.h"

#include "unk_02034354_internal.h"
#include "unk_02035900.h"

void sub_0203453C(int netId, int param1, void *src, void *unused);

void sub_0203453C(int netId, int param1, void *src, void *unused) {
    CommPlayerInfo *playerInfo = (CommPlayerInfo *)src;

    if (!_021D4130) {
        return;
    }

    if (!sub_020373B4(netId)) {
        return;
    }

    MI_CpuCopy8(src, &_021D4130->playerInfo[playerInfo->netId], sizeof(CommPlayerInfo));
    _021D4130->curNetId = playerInfo->netId;

    if (PlayerProfile_IsNameEmpty(_021D4130->trainerInfo[_021D4130->curNetId]) == 1) {
        return;
    }

    if (_021D4130->infoState[_021D4130->curNetId] < INFO_STATE_RECIEVE) {
        _021D4130->infoState[_021D4130->curNetId] = INFO_STATE_BEGIN_RECIEVE;

        if (sub_0203769C() == _021D4130->curNetId) {
            _021D4130->infoState[_021D4130->curNetId] = INFO_STATE_END_RECIEVE;
        }
    }
}
