#include "global.h"

#include "unk_02035900.h"
#include "unk_02033AE0.h"
#include "unk_02034B0C.h"
#include "unk_02037C94.h"
#include "unk_02035900_work.h"

void sub_02037498(u8 param0);
u8 sub_020374AC(int param0);
u16 sub_020374C0(int param0);

int sub_02037454(void) {
    int v0 = 0, i;

    for (i = 0; i < (7 + 1); i++) {
        if (sub_020373B4(i)) {
            v0++;
        }
    }

    return v0;
}

BOOL sub_02037474(void) {
    if (_021D4140.sCommunicationSystem) {
        if (sub_02034044(sub_0203993C())) {
            return TRUE;
        }
    }

    return sub_02035650();
}

void sub_02037498(u8 param0) {
    _021D4140.sCommunicationSystem->sendSpeed = param0;
}

u8 sub_020374AC(int param0) {
    return _021D4140.sCommunicationSystem->recvSpeed[param0];
}

u16 sub_020374C0(int param0) {
    int v0;

    if (!_021D4140.sCommunicationSystem) {
        return 0;
    }

    v0 = _021D4140.sCommunicationSystem->unk_644[param0];
    _021D4140.sCommunicationSystem->unk_644[param0] = 0;

    return v0;
}

void sub_020374E4(void) {
    if (_021D4140.sCommunicationSystem) {
        _021D4140.sCommunicationSystem->sendHeldKeys |= 0x8000;
    }
}
