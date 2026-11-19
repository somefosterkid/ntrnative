#include "global.h"

#include "unk_02035900.h"
#include "unk_02035900_work.h"

SysTask *sub_02035DD8();

s32 sub_02037594(void);

extern void *_0210F900;

void sub_02035F4C(void);

void sub_02035F4C(void) {
    s32 v0;

    v0 = 0;
    if (sub_0203769C() == 0) {
        if (*(u8 *)(&_0210F900) != 4) {
            return;
        }
        goto block_4;
    }
    if (*(u8 *)((u8 *)(&_0210F900) + 1) == 4) {
block_4:
        if (_021D4140.sCommunicationSystem->transmissionType == 2) {
            _021D4140.sCommunicationSystem->transmissionType = 0U;
            v0 = 1;
        }
        if (_021D4140.sCommunicationSystem->transmissionType == 3) {
            v0 = 1;
            _021D4140.sCommunicationSystem->transmissionType = 1U;
        }
        if (v0 != 0) {
            sub_02035DD8();
        }
        sub_02037594();
    }
}
