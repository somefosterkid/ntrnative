#include "global.h"

#include "error_handling.h"
#include "unk_0202C034.h"
#include "unk_02034354.h"
#include "unk_02034354_internal.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"

void sub_02034960(int param0, int val);

void sub_02034960(int param0, int val) {
    int netId;
    int v1, v2;

    if (_021D4130 == NULL) {
        return;
    }

    if (param0 != 2) {
        v1 = sub_020378AC(sub_0203769C()) & 0x1;
    }

    for (netId = 0; netId < sub_02037454(); netId++) {
        if (sub_020373B4(netId) && (_021D4130->infoState[netId] != 0)) {
            if (param0 == 0) {
                v2 = sub_020378AC(netId) & 0x1;

                if (v1 != v2) {
                    _021D4130->playerRecord[netId].win += val;
                }
            } else if (param0 == 1) {
                v2 = sub_020378AC(netId) & 0x1;

                if (v1 != v2) {
                    _021D4130->playerRecord[netId].lose += val;
                }
            } else {
                _021D4130->playerRecord[netId].trades += val;
            }
        }
    }
}

void sub_0202C4F0(WiFiList *wiFiList, int param1, int param2, int param3, int param4);

void sub_02034A20(SaveData *saveData) {
    WiFiList *v0 = sub_0202C6F4(saveData);
    int netId, v2, v3;

    for (netId = 0; netId < sub_02037454(); netId++) {
        DWCFriendData *friendData = sub_0203484C(netId);

        if (friendData == 0L) {
            continue;
        }

        v2 = sub_0203A084(saveData, friendData, &v3);

        switch (v2) {
        case 0:
        case 1:
            ((v3 >= 0) ? (void)0 : GF_AssertFail());

            sub_0202C4F0(v0, v3, _021D4130->playerRecord[netId].win, _021D4130->playerRecord[netId].lose, _021D4130->playerRecord[netId].trades);
            break;
        }
    }

    for (netId = 0; netId < 8; netId++) {
        _021D4130->playerRecord[netId].win = 0;
        _021D4130->playerRecord[netId].lose = 0;
        _021D4130->playerRecord[netId].trades = 0;
    }
}
