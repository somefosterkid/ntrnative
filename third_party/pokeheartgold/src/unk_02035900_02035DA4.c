#include "global.h"
#include "unk_02035900_work.h"

s32 sub_02035AE0();

extern void *_0210F900;

void sub_02035DA4(void);

void sub_02035DA4(void) {
    _021D4140.sCommunicationSystem->unk_6AF = 0;
    _021D4140.sCommunicationSystem->unk_6B0 = 0;
    _021D4140.sCommunicationSystem->unk_664 = 1;
    sub_02035AE0();
    *(s8 *)(&_0210F900) = 4;
    *(s8 *)((u8 *)(&_0210F900) + 1) = 4;
}
