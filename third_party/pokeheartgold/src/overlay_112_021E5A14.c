#include "global.h"

#include "overlay_112_ir_state.h"

s32 ov112_021E594C(void);

s32 ov112_021E59B4(u8 *, s32, s32, u8, s32);

extern u8 _021FF500;


void ov112_021E5A14(s32 arg0);

void ov112_021E5A14(s32 arg0) {
    if ((_021FF9E0.unk_2C != 0) && (_021FF9E0.unk_08 == 0) && (arg0 == 0)) {
        ov112_021E59B4(0, 0, 0xF4, _021FF500, _021FF9E0.unk_04);
    }
    if (_021FF9E0.unk_14 != NULL) {
        ((s32(*)(s32))_021FF9E0.unk_14)(arg0);
    }
    ov112_021E594C();
}
