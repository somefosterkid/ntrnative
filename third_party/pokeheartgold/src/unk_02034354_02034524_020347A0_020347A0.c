#include "global.h"

#include "save.h"
#include "unk_02034354.h"
#include "unk_02034354_internal.h"

int sub_020347A0(void) {
    int netId;
    int cnt = 0;

    for (netId = 0; netId < 8; netId++) {
        switch (_021D4130->infoState[netId]) {
        case INFO_STATE_RECIEVE:
        case INFO_STATE_END_RECIEVE:
            cnt++;
            break;
        }
    }

    return cnt;
}
