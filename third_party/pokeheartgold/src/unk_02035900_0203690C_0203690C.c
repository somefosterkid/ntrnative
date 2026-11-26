#include "global.h"

#include "unk_02033AE0.h"
#include "unk_02037C94.h"
#include "unk_02035900_work.h"

void sub_02033AF0(void *, u8 *, u32, u32);

u8 sub_02035FF0(void);

u32 sub_02036D14(u8 *, s32);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 sub_0203772C(u8);
#else
s32 sub_0203772C(u16);
#endif

void sub_0203690C(s32 param_1, u8 *param_2, u16 param_3);

void sub_0203690C(s32 param_1, u8 *param_2, u16 param_3) {
    u32 uVar1;
    s32 iVar2;
    u32 uVar3;

    _021D4140.sCommunicationSystem->unk_66C[param_1] = _021D4140.sCommunicationSystem->unk_66C[param_1] + -1;
    if (param_2 != NULL) {
        if ((_021D4140.sCommunicationSystem->unk_69E[param_1] == 0) || ((*param_2 & 1) == 0)) {
            _021D4140.sCommunicationSystem->unk_69E[param_1] = 0;
            iVar2 = sub_02035FF0();
            if (iVar2 == 1) {
                uVar1 = sub_0203993C();
                uVar3 = sub_0203772C(uVar1);
                uVar1 = sub_0203993C();
                sub_02033FC4(uVar1);
                if ((*param_2 & 2) == 0) {
                    sub_02033AF0(&_021D4140.sCommunicationSystem->unk_4B0[param_1], param_2, uVar3, 0x5a5);
                }
                _021D4140.sCommunicationSystem->unk_696[param_1] = _021D4140.sCommunicationSystem->unk_696[param_1] + 1;
                return;
            }
            sub_02036D14(param_2, param_1);
            if ((*param_2 & 2) == 0) {
                sub_02033AF0(&_021D4140.sCommunicationSystem->sendRingClient[param_1], param_2 + 1, 0xb, 0x5ae);
            }
        }
    }
    return;
}
