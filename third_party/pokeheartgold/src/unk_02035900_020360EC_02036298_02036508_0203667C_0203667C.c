#include "global.h"

#include "unk_02033AE0.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_02035900_work.h"

int sub_02033298(void);

u8 sub_02035FF0(void);

BOOL sub_02036438(int param0);

void sub_02036508(void);

BOOL sub_02036630(void);

s32 ov00_021E602C(void *, s32);

s32 sub_02036F30(void *);

extern u8 _0210F900;

void sub_0203667C(void);

void sub_0203667C(void) {
    s32 temp_r6;
    s32 var_r4;

    temp_r6 = sub_02033FC4((s16)sub_0203993C()) + 1;
    if (sub_02034044((s32)sub_0203993C()) != 0) {
        if (sub_020373B4(0U) != 0) {
            if (_021D4140.sCommunicationSystem->unk_664 != 0) {
                if (sub_02036630() != 0) {
                    if (_0210F900 == 4) {
                        if (sub_02035FF0() == 1) {
                            sub_02036438(0);
                        }
                        _0210F900 = 2;
                    }
                    goto block_12;
                }
            } else if ((_0210F900 != 4) || (sub_02035FF0() != 1) || (sub_02036438(0) != 0)) {
                _0210F900 = 2;
block_12:
                if (ov00_021E602C(_021D4140.sCommunicationSystem->sendBufferServer[0], 0xC0) != 0) {
                    var_r4 = 0;
                    _0210F900 = 4;
                    if (temp_r6 > 0) {
                        do {
                            if (sub_020373B4(var_r4) != 0) {
                                _021D4140.sCommunicationSystem->unk_66C[var_r4]++;
                            }
                            var_r4 += 1;
                        } while ((s32)var_r4 < temp_r6);
                    }
                }
            }
        }
    } else if ((sub_02033298() == 4) && (_0210F900 == 4) && (sub_02036630() != 0)) {
        if (sub_02035FF0() == 0) {
            sub_02036F30(_021D4140.sCommunicationSystem->sendBufferServer[_021D4140.sCommunicationSystem->unk_6B0]);
            sub_02036F30(_021D4140.sCommunicationSystem->sendBufferServer[1 - _021D4140.sCommunicationSystem->unk_6B0]);
        }
        _0210F900 = 0;
        sub_02036508();
    }
}
