#include "global.h"

#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_02035900_work.h"

s32 sub_02036FA8(void);

s32 sub_02036FA8(void) {
    u8 v0;

    v0 = _021D4140.sCommunicationSystem->unk_6BB;
    if (v0 == 0) {
        return 0;
    }
    if (((s32) * (u8 *)(&_021D4140.sCommunicationSystem->unk_6BC) % (s32)v0) == 0) {
        return 1;
    }
    return 0;
}

s32 sub_02033DF0(CommQueueMan *, s32, void *, s32, s32, s32);

s32 sub_02037974(void);

s32 sub_02036FD8(s32 arg0, void *arg1, s32 arg2);

s32 sub_02036FD8(s32 arg0, void *arg1, s32 arg2) {
    if (sub_020373B4(sub_0203769C()) == 0) {
        return 0;
    }
    if (sub_02033DF0(&_021D4140.sCommunicationSystem->commQueueManSend, arg0, arg1, arg2, 1, 0) != 0) {
        return 1;
    }
    if (sub_0203993C() == 0xA) {
        sub_02037974();
    }
    return 0;
}
