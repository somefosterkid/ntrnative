#include "global.h"

#include "unk_02033AE0.h"
#include "unk_02037C94.h"
#include "unk_02035900_work.h"

s32 sub_02033BC4(void *);

s32 sub_02033C28(void *);

void sub_020371C4(CommRing *ring, int netId, u8 *buffer, CommRecvPackage *param3);

void sub_02037334(void);

void sub_02037334(void) {
    s32 sp0;
    s32 temp_r0;
    s32 var_r4;

    if ((_021D4140.sCommunicationSystem != NULL) && (_021D4140.sCommunicationSystem->unk_6BA == 0)) {
        temp_r0 = sub_02033FC4((s16)sub_0203993C()) + 1;
        var_r4 = 0;
        sp0 = temp_r0;
        if (temp_r0 > 0) {
            do {
                sub_02033C28(&_021D4140.sCommunicationSystem->sendRingClient[var_r4]);
                if (sub_02033BC4(&_021D4140.sCommunicationSystem->sendRingClient[var_r4]) > 0) {
                    sub_020371C4(&_021D4140.sCommunicationSystem->sendRingClient[var_r4], var_r4, _021D4140.sCommunicationSystem->tempBuffer, &_021D4140.sCommunicationSystem->commRecvServer[var_r4]);
                }
                var_r4 += 1;
            } while (var_r4 < sp0);
        }
    }
}
