#include "global.h"

#include "overlay_112_ir_state.h"


void ov112_021E5D38(void *arg0);

void ov112_021E5D38(void *arg0) {
    _021FF9E0.unk_0C = arg0;
}

void ov112_021E5D44(void *arg0);

void ov112_021E5D44(void *arg0) {
    _021FF9E0.unk_1C = arg0;
}

void ov112_021E5D50(void *arg0);

void ov112_021E5D50(void *arg0) {
    _021FF9E0.unk_14 = arg0;
}

extern s8 _021FF500;

void ov112_021E5D5C(s8 arg0);

void ov112_021E5D5C(s8 arg0) {
    _021FF500 = arg0;
}


void ov112_021E5D68(u8 *arg0);

void ov112_021E5D68(u8 *arg0) {
    if (3 & (u32)arg0) {
        OS_Halt();
    }
    ov112_021FFAA4.unk_48 = arg0;
    ov112_021FFAA4.unk_44 = arg0 + 0xCD80;
}
