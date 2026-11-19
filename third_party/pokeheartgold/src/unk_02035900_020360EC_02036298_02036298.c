#include "global.h"

#include "unk_02033AE0.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_02035900_work.h"

int sub_02033298(void);

BOOL sub_02036630(void);

BOOL sub_02036E60(u8 *param0);

s32 sub_02036FA8(void);

s32 ov00_021E5F84(u8 *, s32);

s32 ov00_021E60E8(u8 *, s32 arg1);

s32 sub_02034084(int);

s32 sub_02036AD8(void);

extern volatile u8 _0210F900[2];

/* Platinum: static volatile u8 Unk_02100A1C, Unk_02100A1D */

void sub_02036298(void);

void sub_02036298(void) {
    if (sub_02034084(sub_0203993C())) {
        if (_021D4140.sCommunicationSystem->wifiConnected) {
            if (_021D4140.sCommunicationSystem->unk_664) {
                if (!sub_02036630()) {
                    return;
                }
                if (_0210F900[1] == 4) {
                    sub_02036E60(_021D4140.sCommunicationSystem->sendBuffer[0]);
                    _0210F900[1] = 2;
                }
            } else {
                if (_0210F900[1] == 4) {
                    if (!sub_02036E60(_021D4140.sCommunicationSystem->sendBuffer[0])) {
                        return;
                    }
                    _0210F900[1] = 2;
                }
            }
            if (sub_02036FA8()) {
                return;
            }
            if (ov00_021E60E8(_021D4140.sCommunicationSystem->sendBuffer[0], 38)) {
                int i;
                int max = sub_02033FC4((u32)((u16)sub_0203993C())) + 1;

                for (i = 0; i < max; i++) {
                    if (sub_020373B4(i)) {
                        _021D4140.sCommunicationSystem->unk_66C[i]++;
                    }
                }
                _0210F900[1] = 4;
            }
        }
    } else if (sub_02034044(sub_0203993C())) {
        if (_021D4140.sCommunicationSystem->wifiConnected) {
            if (_021D4140.sCommunicationSystem->unk_664) {
                if (_021D4140.sCommunicationSystem->unk_668 > 3) {
                    return;
                }
                if (_0210F900[1] == 4) {
                    sub_02036E60(_021D4140.sCommunicationSystem->sendBuffer[0]);
                    _0210F900[1] = 2;
                }
            } else {
                if (_0210F900[1] == 4) {
                    if (!sub_02036E60(_021D4140.sCommunicationSystem->sendBuffer[0])) {
                        return;
                    }
                    _0210F900[1] = 2;
                }
            }
            if (sub_02036FA8()) {
                return;
            }
            if (ov00_021E5F84(_021D4140.sCommunicationSystem->sendBuffer[0], 38)) {
                _0210F900[1] = 4;
                _021D4140.sCommunicationSystem->unk_668++;
            }
        }
    } else if (sub_02033298() == 4 && sub_020373B4(sub_0203769C())) {
        if (_0210F900[1] == 4 && _021D4140.sCommunicationSystem->unk_668 <= 3) {
            sub_02036E60(_021D4140.sCommunicationSystem->sendBuffer[_021D4140.sCommunicationSystem->unk_6AF]);
            sub_02036E60(_021D4140.sCommunicationSystem->sendBuffer[1 - _021D4140.sCommunicationSystem->unk_6AF]);
            _0210F900[1] = 0;
        }
        sub_02036AD8();
    }
}
