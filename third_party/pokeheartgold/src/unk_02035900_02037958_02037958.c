#include "global.h"
#include "unk_02035900_work.h"

s32 sub_02037958(void);

s32 sub_02037958(void) {
    if (_021D4140.sCommunicationSystem->unk_6B1 != 0) {
        return 1;
    }
    return 0;
}

void sub_02037974(void);

void sub_02037974(void) {
    _021D4140.sCommunicationSystem->unk_6B8 = 1;
}

void sub_02037988(void);

void sub_02037988(void) {
    if (_021D4140.sCommunicationSystem != NULL) {
        _021D4140.sCommunicationSystem->unk_6B9 = 1;
    }
}
