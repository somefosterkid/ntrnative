#include "global.h"

#include "comm_server_client.h"
#include "unk_02037C94.h"

u8 sub_0203993C(void);


BOOL sub_0203401C(int param0);

void sub_02034C20(WMBssDesc *param0);

void sub_02034C20(WMBssDesc *param0) {
    CommGameInfo *v1;
    int v2 = sub_0203993C();
    int v3 = sub_02039954();

    v1 = (CommGameInfo *)param0->gameInfo.userGameInfo;

    if (v2 == 14) {
        (void)0;
    } else if (sub_0203401C(v1->unk_04) && sub_0203401C(v2)) {
        (void)0;
    } else if (v1->unk_54 && v1->unk_04 == 10) {
        return;
    } else if (v1->unk_04 != v2) {
        return;
    }

    if (v2 != 14 && v1->unk_05 != v3) {
        return;
    }

    MI_CpuCopy8(param0, &_021D4134.sCommServerClient->unk_54, sizeof(WMBssDesc));
    _021D4134.sCommServerClient->unk_D95_6 = 1;
}
