#include "global.h"

#include "unk_02033AE0.h"
#include "unk_02037C94.h"
#include "unk_02035900_work.h"

void sub_020378E4(s32 a0);

void sub_020378E4(s32 a0) {
    u8 bVar1;
    BOOL BVar2;
    s32 iVar3;

    bVar1 = sub_0203993C();
    BVar2 = sub_02034044((u32)bVar1);
    if (((BVar2 != 0) && (_021D4140.sCommunicationSystem->unk_664 != a0)) && (_021D4140.sCommunicationSystem->unk_664 = a0, a0 != 0)) {
        iVar3 = 0;
        _021D4140.sCommunicationSystem->unk_668 = 0;
        do {
            _021D4140.sCommunicationSystem->unk_66C[iVar3] = 0;
            iVar3 = iVar3 + 1;
        } while (iVar3 < 8);
    }
    return;
}
