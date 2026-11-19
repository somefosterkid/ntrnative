#include "global.h"

#include "unk_02033AE0.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"

BOOL sub_02034EF0(BOOL param0, BOOL param1, BOOL param2);
typedef void (*WirelessManagerConnectFunc)(int);
void sub_02033A90(WirelessManagerConnectFunc callback);
BOOL sub_02035900(BOOL shouldAlloc, int maxPacketSize);
void sub_02035ED0(int param0);

BOOL sub_02035ED8(BOOL param0, BOOL param1, int param2, BOOL param3) {
    BOOL ret = 1;

    if (!sub_02034044(sub_0203993C())) {
        ret = sub_02034EF0(param0, param1, param3);
        sub_02033A90(sub_02035ED0);
    }

    sub_02035900(param0, param2);
    return ret;
}
