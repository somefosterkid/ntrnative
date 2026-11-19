#include "global.h"

#include "error_handling.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_02035900_work.h"

s32 sub_02033DF0(CommQueueMan *, u32, void *, u32, u32, u32);

u8 sub_02035FF0(void);

u32 sub_02036FD8(u32, void *, u32);

void sub_02037974(void);

u32 sub_02037088(u32 param_1, void *param_2, u32 param_3);

u32 sub_02037088(u32 param_1, void *param_2, u32 param_3) {
    u8 uVar1;
    u16 uVar2;
    BOOL BVar3;
    s32 iVar4;
    u32 uVar5;

    uVar2 = sub_0203769C();
    if (uVar2 != 0) {
        GF_AssertFail();
        return 0;
    }
    BVar3 = sub_020373B4(0);
    if (BVar3 == 0) {
        return 0;
    }
    iVar4 = sub_02035FF0();
    if (iVar4 == 1) {
        uVar5 = sub_02036FD8(param_1, param_2, param_3);
        return uVar5;
    }
    iVar4 = sub_02033DF0(&_021D4140.sCommunicationSystem->commQueueManSendServer, param_1, param_2, param_3, 1, 0);
    if (iVar4 != 0) {
        return 1;
    }
    uVar1 = sub_0203993C();
    if (uVar1 == 0xa) {
        sub_02037974();
    }
    return 0;
}
