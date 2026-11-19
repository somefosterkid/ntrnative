#include "global.h"
#include "unk_02035900_work.h"

s32 sub_02033BE4(void *);

void sub_02037190(void);

void sub_02034170(int param0, int param1, int param2, void *param3);
void sub_020371A8(int netId, int command, int param2, void *param3, CommRecvPackage *param4);

void sub_02037190(void) {
    sub_02033BE4(&_021D4140.sCommunicationSystem->sendRing);
}

void sub_020371A8(int netId, int command, int param2, void *param3, CommRecvPackage *param4) {
    sub_02034170(netId, command, param2, param3);
    param4->unk_0A = 0xee;
    param4->unk_08 = 0xffff;
    param4->unk_04 = 0L;
    param4->unk_00 = 0;
}
