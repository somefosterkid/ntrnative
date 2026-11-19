#include "global.h"

#include "overlay_112_ir_state.h"


extern u8 ov112_021FFA18;

s32 ov112_021E5A84(u32 arg0);

s32 ov112_021E5A84(u32 arg0) {
    u32 i;

    if (arg0 == 1) {
        if (_021FF9E0.unk_2C != 0) {
            return 1;
        }
        if (ov112_021FFA18 != 0xFC) {
            return 1;
        }
        return 0;
    }
    switch (arg0) {
    case 2:
    case 3:
        for (i = 0; i < 4; i++) {
        }
        for (i = 0; i < arg0; i++) {
        }
        return 1;
    default:
        return 0;
    }
}
