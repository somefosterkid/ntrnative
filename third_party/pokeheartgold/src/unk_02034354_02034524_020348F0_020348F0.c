#include "global.h"

#include "unk_02034354_internal.h"
#include "unk_02035900.h"

BOOL sub_020348F0(void);

BOOL sub_020348F0(void) {
    int netId, i;

    for (netId = 0; netId < 8 - 1; netId++) {
        if (sub_020373B4(netId) && (_021D4130->infoState[netId] != 0)) {
            if (sub_020373B4(netId + 1) && (_021D4130->infoState[netId + 1] != 0)) {
                for (i = 0; i < 32; i++) {
                    if (_021D4130->playerInfo[netId].regulationBuffer.bytes[i] != _021D4130->playerInfo[netId + 1].regulationBuffer.bytes[i]) {
                        return 0;
                    }
                }
            }
        }
    }

    return 1;
}
