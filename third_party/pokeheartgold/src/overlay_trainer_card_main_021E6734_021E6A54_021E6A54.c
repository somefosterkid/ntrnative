#include "global.h"

#include "unk_02005D10.h"

extern u8 gSystem;

u32 ov51_021E6A54(u8 *param_1);

u32 ov51_021E6A54(u8 *param_1) {
    if ((((u32 *)&gSystem)[0x12] & 1) != 0) {
        if ((*(s32 *)(param_1 + 0x30f4) != 0) && (((u32)(*(u8 *)(param_1 + 0x343a) << 0x1e) >> 0x1f) == 0)) {
            PlaySE(0x5dc);
            return 4;
        }
    } else {
        if ((((u32 *)&gSystem)[0x12] & 2) != 0) {
            PlaySE(0x940);
            return 5;
        }
    }
    if ((((u32 *)&gSystem)[0x12] & 0x30) != 0) {
        return 3;
    }
    return 0;
}
