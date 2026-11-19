#include "global.h"

#include "unk_02035900.h"
#include "unk_02035900_work.h"

void sub_02037504(void) {
    if (_021D4140.sCommunicationSystem != NULL) {
        _021D4140.sCommunicationSystem->sendHeldKeys = 0;
    }
}

s32 sub_0203751C(void);

s32 sub_0203751C(void) {
    if (_021D4140.sCommunicationSystem != NULL) {
        return 0x8000 & _021D4140.sCommunicationSystem->sendHeldKeys;
    }
    return 1;
}

s32 sub_02033DF0(void *, s32, void *, s32, s32, s32);

u8 sub_02035FF0(void);

void sub_0203753C(s32 arg0, u8 *arg1, s32 arg2);

void sub_0203753C(s32 arg0, u8 *arg1, s32 arg2) {
    if (sub_02035FF0() == 1) {
        sub_02033DF0(&_021D4140.sCommunicationSystem->commQueueManSend, arg0, arg1, arg2, 1, 0);
        return;
    }
    sub_02033DF0(&_021D4140.sCommunicationSystem->commQueueManSendServer, arg0, arg1, arg2, 1, 0);
}
