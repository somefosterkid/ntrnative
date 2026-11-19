#include "global.h"

#include "unk_02034354.h"
#include "unk_02034354_internal.h"
#include "unk_02035900.h"

BOOL sub_020347CC(void);

BOOL sub_020347CC(void) {
    int netId;
    BOOL ret = 0;
    if (_021D4130) {
        netId = 0;
        if (sub_02037454() == netId) {
            return ret;
        }
        for (netId = 0; netId < 8; netId++) {
            if ((!sub_020373B4(netId)) && (_021D4130->infoState[netId] != 0)) {
                sub_020346E8(netId);
                ret = 1;
            }
        }
    }
    return ret;
}
