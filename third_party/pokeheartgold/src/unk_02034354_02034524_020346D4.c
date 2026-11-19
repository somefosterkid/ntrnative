#include "global.h"

#include "player_data.h"
#include "unk_02034354_internal.h"

s32 sub_020346D4(void);

s32 sub_020346D4(void) {
    return _021D4130->dataRecvFlag;
}

void sub_020346E8(s32 netId);

void sub_020346E8(s32 netId) {
    PlayerProfile_Init(_021D4130->trainerInfo[netId]);
    _021D4130->infoState[netId] = INFO_STATE_EMPTY;
}

s32 sub_02034714(s32 netId);

s32 sub_02034714(s32 netId) {
    if (_021D4130->infoState[netId] == INFO_STATE_BEGIN_RECIEVE) {
        return 1;
    }
    return 0;
}

s32 sub_02034730(s32 netId);

s32 sub_02034730(s32 netId) {
    s32 v1;
    u8 v0;

    v1 = 1;
    v0 = _021D4130->infoState[netId];
    if ((v0 != INFO_STATE_RECIEVE) && (v0 != INFO_STATE_BEGIN_RECIEVE)) {
        v1 = 0;
    }
    return v1;
}

s32 sub_02034750(s32 netId);

s32 sub_02034750(s32 netId) {
    if (_021D4130->infoState[netId] == INFO_STATE_RECIEVE) {
        return 1;
    }
    return 0;
}

void sub_0203476C(s32 netId);

void sub_0203476C(s32 netId) {
    _021D4130->infoState[netId] = INFO_STATE_RECIEVE;
}

s32 sub_02034780(void);

s32 sub_02034780(void) {
    s32 netId;

    for (netId = 0; netId < 8; netId++) {
        if (_021D4130->infoState[netId] == INFO_STATE_BEGIN_RECIEVE) {
            return netId;
        }
    }
    return 0xFF;
}
