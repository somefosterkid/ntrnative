#include "global.h"

#include "system.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_02035900_work.h"

s32 sub_02034084(int param0);

void sub_020355C8(u16);

s32 sub_02035E9C(void);

s32 sub_02035F4C(void);

u8 sub_02035FF0(void);

s32 sub_0203611C(void);

s32 sub_02036298(void);

s32 sub_0203667C(void);

s32 sub_02036BE4();

s32 sub_020372E4(void);

s32 sub_02037334(void);

s32 sub_02037ADC(void);

BOOL sub_02036144(void) {
    sub_0203817C();
    if (_021D4140.sCommunicationSystem != NULL) {
        if ((_021D4140.sCommunicationSystem->unk_6B9) == 0) {
            _021D4140.sCommunicationSystem->unk_6BC = (u8)((_021D4140.sCommunicationSystem->unk_6BC) + 1);
            _021D4140.unk_01 = 0;
            sub_02035F4C();
            _021D4140.sCommunicationSystem->sendHeldKeys |= gSystem.heldKeys & 0x7FFF;
            sub_02036BE4();
            sub_02036298();
            _021D4140.sCommunicationSystem->sendHeldKeys = (u16)(0x8000 & (_021D4140.sCommunicationSystem->sendHeldKeys));
            if (sub_02035FF0() == 0) {
                sub_020372E4();
            }
            if ((sub_0203769C() == 0) && (sub_020373B4(0U) != 0)) {
                if (sub_02034084(sub_0203993C()) == 0) {
                    sub_0203667C();
                }
            }
            if ((sub_0203769C() == 0) || (sub_02035FF0() == 1)) {
                sub_02037334();
            }
            _021D4140.unk_01 = 1;
        }
        sub_020355C8(_021D4140.sCommunicationSystem->unk_694);
        if (sub_0203769C() == 0) {
            sub_02035E9C();
        }
        sub_0203611C();
    } else {
        sub_020355C8(0U);
    }
    sub_02039A00(0);
    sub_02037ADC();
    return 1;
}
