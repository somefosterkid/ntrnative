#include "global.h"

#include "unk_02035900.h"
#include "unk_02035900_work.h"

s32 sub_02035DA4(void);

void sub_0203622C(void) {
    u8 v0;

    v0 = _021D4140.unk_01;
    _021D4140.unk_01 = 0U;
    if (_021D4140.sCommunicationSystem != NULL) {
        sub_02035DA4();
    }
    _021D4140.unk_01 = v0;
}

void sub_0203624C(void) {
    u8 v0;

    v0 = _021D4140.unk_01;
    _021D4140.unk_01 = 0U;
    if (_021D4140.sCommunicationSystem != NULL) {
        _021D4140.sCommunicationSystem->transmissionType = 1;
        sub_02035DA4();
    }
    _021D4140.unk_01 = v0;
}

s32 sub_02034E2C(void);

void sub_02036274(void) {
    u8 v0;

    v0 = _021D4140.unk_01;
    _021D4140.unk_01 = 0U;
    if (_021D4140.sCommunicationSystem != NULL) {
        sub_02035DA4();
        sub_02034E2C();
    }
    _021D4140.unk_01 = v0;
}
