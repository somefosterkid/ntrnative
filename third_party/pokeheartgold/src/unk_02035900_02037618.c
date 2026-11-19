#include "global.h"

#include "unk_02035900.h"
#include "unk_02035900_work.h"

void sub_02037618(u32 param_1, u32 param_2, u8 *param_3);

void sub_02035FA8(int);
void sub_02037668(u32 param_1, u32 param_2, u8 *param_3);

void sub_02037618(u32 param_1, u32 param_2, u8 *param_3) {
    u16 uVar1;

    uVar1 = sub_0203769C();
    if (uVar1 == 0) {
        _021D4140.sCommunicationSystem->transmissionState = 1;
        _021D4140.sCommunicationSystem->unk_6AC = *param_3;
    }
    return;
}

void sub_02037640(u32 param_1, u32 param_2, u8 *param_3);

void sub_02037640(u32 param_1, u32 param_2, u8 *param_3) {
    u16 uVar1;

    uVar1 = sub_0203769C();
    if (uVar1 != 0) {
        _021D4140.sCommunicationSystem->unk_6AC = *param_3;
        _021D4140.sCommunicationSystem->transmissionState = 3;
    }
    return;
}

void sub_02037668(u32 param_1, u32 param_2, u8 *param_3) {
    u16 uVar1;

    uVar1 = sub_0203769C();
    if ((uVar1 == 0) && (_021D4140.sCommunicationSystem->transmissionState == 2)) {
        sub_02035FA8(*param_3);
        _021D4140.sCommunicationSystem->transmissionState = 0;
    }
    return;
}
