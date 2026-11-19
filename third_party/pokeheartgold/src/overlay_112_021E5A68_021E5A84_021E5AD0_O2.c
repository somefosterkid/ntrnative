#include "global.h"

#include "overlay_112_ir_state.h"


void ov112_021E5A14(s32 arg0);

BOOL ov112_021E5AD0(void);

BOOL ov112_021E5AD0(void) {
    u64 diff;
    if (_021FF9E0.unk_2C == 0 && _021FF9E0.unk_24 == 0) {
        return FALSE;
    }
    diff = OS_GetTick() - _021FF9E0.unk_30;
    if ((diff << 6) / 0x82EA < 100) {
        return FALSE;
    }
    ov112_021E5A14(1);
    _021FF9E0.unk_24 = 0;
    return TRUE;
}
