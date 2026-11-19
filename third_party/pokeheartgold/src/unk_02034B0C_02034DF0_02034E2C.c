#include "global.h"

#include "comm_server_client.h"

void sub_02034E2C(void);

void sub_02034E2C(void) {
    s32 iVar2;
    s32 iVar3;

    iVar2 = 0;
    iVar3 = 0;
    do {
        iVar2 = iVar2 + 1;
        _021D4134.sCommServerClient->unk_D44[iVar3] = 0;
        iVar3 = iVar3 + 1;
    } while (iVar2 < 0x10);
    MI_CpuFill8(_021D4134.sCommServerClient->unk_114, 0, sizeof(_021D4134.sCommServerClient->unk_114));
    return;
}
