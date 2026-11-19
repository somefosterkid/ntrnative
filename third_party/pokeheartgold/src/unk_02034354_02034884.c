#include "global.h"

#include "unk_02034354.h"
#include "unk_02034354_internal.h"

u16 *sub_02034884(int netId) {
    if (_021D4130->infoState[netId] != INFO_STATE_EMPTY) {
        return _021D4130->playerInfo[netId].unk_4C;
    }
    return NULL;
}

int sub_020348A8(int netId) {
    if (_021D4130->infoState[netId] != INFO_STATE_EMPTY) {
        return _021D4130->playerInfo[netId].country;
    }
    return 0;
}

int sub_020348CC(int netId) {
    if (_021D4130->infoState[netId] != INFO_STATE_EMPTY) {
        return _021D4130->playerInfo[netId].region;
    }
    return 0;
}
