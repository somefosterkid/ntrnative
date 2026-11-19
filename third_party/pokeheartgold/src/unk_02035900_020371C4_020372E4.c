#include "global.h"
#include "unk_02035900_work.h"

s32 sub_02033BC4(void *);

s32 sub_02033C28(void *);

s32 sub_020371C4(void *, s32, u8 *, void *);

void sub_020372E4(void);

void sub_020372E4(void) {
    if ((_021D4140.sCommunicationSystem != NULL) && (_021D4140.sCommunicationSystem->unk_6BA == 0)) {
        sub_02033C28(&_021D4140.sCommunicationSystem->recvRing);
        if (sub_02033BC4(&_021D4140.sCommunicationSystem->recvRing) > 0) {
            sub_020371C4(&_021D4140.sCommunicationSystem->recvRing, 0, _021D4140.sCommunicationSystem->tempBuffer, &_021D4140.sCommunicationSystem->commRecvClient);
        }
    }
}
