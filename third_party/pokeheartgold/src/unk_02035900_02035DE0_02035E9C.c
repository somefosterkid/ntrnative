#include "global.h"

#include "unk_02035900.h"
#include "unk_02035900_work.h"

void sub_02035DE0(s32 param_1);

void sub_02035E9C(void);

void sub_02035E9C(void) {
    BOOL BVar1;
    s32 iVar2;
    iVar2 = 1;
    do {
        BVar1 = sub_020373B4((u16)iVar2);
        if ((BVar1 == 0) && (_021D4140.sCommunicationSystem->unk_69E[iVar2] == 0)) {
            sub_02035DE0(iVar2);
        }
        iVar2 = iVar2 + 1;
    } while (iVar2 < 8);
    return;
}
