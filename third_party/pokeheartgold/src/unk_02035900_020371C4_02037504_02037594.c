#include "global.h"

#include "unk_02035900.h"
#include "unk_02035900_work.h"

s32 sub_02035FA8(int);

u8 sub_02035FF0(void);

s32 sub_02037108(s32, void *, s32);

void sub_02037594(void);

void sub_02037594(void) {
    s32 var_r0;
    u8 temp_r0;

    if (_021D4140.sCommunicationSystem != NULL) {
        temp_r0 = _021D4140.sCommunicationSystem->transmissionState;
        switch (temp_r0) {
        case 1:
            if (sub_02035FF0() == 1) {
                var_r0 = sub_020376E0(0xB, &_021D4140.sCommunicationSystem->unk_6AC);
            } else {
                var_r0 = sub_02037108(0xB, &_021D4140.sCommunicationSystem->unk_6AC, 1);
            }
            if (var_r0 != 0) {
                _021D4140.sCommunicationSystem->transmissionState = 2U;
                return;
            }
            break;
        case 3:
            if (sub_020376E0(0xC, &_021D4140.sCommunicationSystem->unk_6AC) != 0) {
                sub_02035FA8(_021D4140.sCommunicationSystem->unk_6AC);
                _021D4140.sCommunicationSystem->transmissionState = 0U;
            }
            break;
        }
    }
}
