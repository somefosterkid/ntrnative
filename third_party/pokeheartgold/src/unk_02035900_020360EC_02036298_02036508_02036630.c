#include "global.h"

#include "unk_02033AE0.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_02035900_work.h"

BOOL sub_02036630(void);

BOOL sub_02036630(void) {
    u8 bVar1;
    s32 sVar2;
    BOOL BVar3;
    s32 iVar4;

    bVar1 = sub_0203993C();
    sVar2 = sub_02033FC4((s16)bVar1);
    iVar4 = 1;
    if (1 < sVar2 + 1) {
        do {
            BVar3 = sub_020373B4((u16)iVar4);
            if ((BVar3 != 0) && (3 < _021D4140.sCommunicationSystem->unk_66C[iVar4])) {
                return 0;
            }
            iVar4 = iVar4 + 1;
        } while (iVar4 < sVar2 + 1);
    }
    return 1;
}
