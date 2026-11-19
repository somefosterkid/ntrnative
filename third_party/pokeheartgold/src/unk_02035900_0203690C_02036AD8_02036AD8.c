#include "global.h"

#include "unk_02032844.h"
#include "unk_02033AE0.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_02035900_work.h"

int sub_02033298(void);

void sub_0203690C(s32 param0, u8 *_buffer, u16 param2);

void sub_02036AA0(s32 arg0);

s32 sub_0203772C(u16 arg0);

s32 sub_02033800(void *, s32, s32, void (*)(s32));

extern volatile u8 _0210F900[2];

/* Platinum: static volatile u8 Unk_02100A1C, Unk_02100A1D (communication_system.c) */

void sub_02036AD8(void);

void sub_02036AD8(void) {
    s32 temp_r4;

    if (_021D4140.sCommunicationSystem != NULL) {
        if (sub_02034044((s32)sub_0203993C()) == 0) {
            temp_r4 = sub_0203772C((int)sub_0203993C());
            sub_02033FC4((int)sub_0203993C());
            if (sub_02033298() == 4) {
                if (sub_020373B4(sub_0203769C()) == 0) {
                    sub_0203769C();
                    return;
                }
                if (_0210F900[1] == 2 || _0210F900[1] == 0) {
                    if (sub_0203769C() != 0) {
                        _0210F900[1] = (_0210F900[1] + 1);
                        if (sub_02033800(_021D4140.sCommunicationSystem->sendBuffer[_021D4140.sCommunicationSystem->unk_6AF], (u16)temp_r4, 0xE, sub_02036AA0) == 0) {
                            _0210F900[1] = (_0210F900[1] - 1);
                            return;
                        }
                        _021D4140.sCommunicationSystem->unk_6AF = (1 - _021D4140.sCommunicationSystem->unk_6AF);
                        _021D4140.sCommunicationSystem->unk_668 = (s32)(_021D4140.sCommunicationSystem->unk_668 + 1);
                        return;
                    }
                    if (sub_02033250() & 0xFFFE) {
                        _0210F900[1] = (_0210F900[1] + 1);
                        sub_02036AA0(1);
                        sub_0203690C(0U, _021D4140.sCommunicationSystem->sendBuffer[_021D4140.sCommunicationSystem->unk_6AF], temp_r4);
                        _021D4140.sCommunicationSystem->unk_6AF = (1 - _021D4140.sCommunicationSystem->unk_6AF);
                        _021D4140.sCommunicationSystem->unk_668 = (s32)(_021D4140.sCommunicationSystem->unk_668 + 1);
                    }
                }
            }
        }
    }
}
