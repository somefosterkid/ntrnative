#include "global.h"

#include "unk_02034354_internal.h"
#include "unk_02035900.h"

void sub_020357C4(u8 *, u32);

void sub_020345D0(u32 netId, u32 unused, const void *src);

void sub_020345D0(u32 netId, u32 unused, const void *src) {
    u16 uVar1;

    if (_021D4130 != NULL) {
        MI_CpuCopy8(src, &_021D4130->playerInfo[netId], sizeof(CommPlayerInfo));
        sub_020357C4(_021D4130->playerInfo[netId].macAddress, netId);
        _021D4130->infoState[netId] = INFO_STATE_BEGIN_RECIEVE;
        uVar1 = sub_0203769C();
        if (netId == uVar1) {
            _021D4130->infoState[netId] = INFO_STATE_END_RECIEVE;
            return;
        }
        _021D4130->dataRecvFlag = 1;
    }
    return;
}
