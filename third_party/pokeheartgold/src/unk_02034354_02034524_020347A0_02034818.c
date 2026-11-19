#include "global.h"

#include "save.h"
#include "unk_02034354_internal.h"
#include "unk_02037C94.h"

PlayerProfile *sub_02034818(int netId);

PlayerProfile *sub_02034818(int netId) {
    if (!_021D4130) {
        return NULL;
    }

    switch (_021D4130->infoState[netId]) {
    case INFO_STATE_BEGIN_RECIEVE:
    case INFO_STATE_RECIEVE:
    case INFO_STATE_END_RECIEVE:
        return _021D4130->trainerInfo[netId];
    }

    return NULL;
}

DWCFriendData *sub_0203484C(int netId);

DWCFriendData *sub_0203484C(int netId) {
    if (_021D4130->infoState[netId] != INFO_STATE_EMPTY) {
        return &_021D4130->playerInfo[netId].friendData;
    }

    return NULL;
}

int sub_02034870(int param0);

int sub_02034870(int param0) {
    return sub_0203A378(_021D4130->saveData, param0);
}
