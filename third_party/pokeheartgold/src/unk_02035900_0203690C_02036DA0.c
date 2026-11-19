#include "global.h"
#include "unk_02035900_work.h"

s32 sub_0203751C(void);

s32 sub_02036DA0(u8 *arg0);

s32 sub_02036DA0(u8 *arg0) {
    u16 v1;
    u8 v0;

    if (_021D4140.sCommunicationSystem->unk_65E != 0) {
        return 0;
    }
    if (sub_0203751C() == 0) {
        return 0;
    }
    v0 = _021D4140.sCommunicationSystem->unk_6B1;
    if (v0 != 0) {
        _021D4140.sCommunicationSystem->unk_6B1 = (u8)(v0 - 1);
    }
    v1 = _021D4140.sCommunicationSystem->sendHeldKeys;
    if (0x40 & v1) {
        *arg0 |= 0x10;
        _021D4140.sCommunicationSystem->unk_6B1 = 8U;
    } else if (0x80 & v1) {
        *arg0 |= 0x14;
        _021D4140.sCommunicationSystem->unk_6B1 = 8U;
    } else if (0x20 & v1) {
        *arg0 |= 0x18;
        _021D4140.sCommunicationSystem->unk_6B1 = 8U;
    } else if (v1 & 0x10) {
        *arg0 |= 0x1C;
        _021D4140.sCommunicationSystem->unk_6B1 = 8U;
    }
    *arg0 |= _021D4140.sCommunicationSystem->sendSpeed << 5;
    return 0;
}
