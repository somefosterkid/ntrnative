#include "global.h"

#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_02035900_work.h"

s32 sub_02033DF0(void *, s32, void *, s32, s32, s32);

s32 sub_02037974(void);

BOOL sub_02037030(int arg0, void *arg1, int arg2) {
    if (sub_020373B4(sub_0203769C()) == 0) {
        return 0;
    }
    if (sub_02033DF0(&_021D4140.sCommunicationSystem->commQueueManSend, arg0, arg1, arg2, 1, 1) != 0) {
        return 1;
    }
    if (sub_0203993C() == 0xA) {
        sub_02037974();
    }
    return 0;
}
