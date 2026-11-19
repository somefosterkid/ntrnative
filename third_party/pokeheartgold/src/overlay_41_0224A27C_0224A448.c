#include "global.h"

#include "error_handling.h"
#include "system.h"

s32 ov41_0224A60C(s32, u32, void *);

extern u8 ov41_0224C094;

void ov41_0224A448(void *arg0);

void ov41_0224A448(void *arg0) {
    u16 var_r4;

    var_r4 = 0xFF;
    if (*(s32 *)((u8 *)arg0 + 0x68) == 0) {
        GF_AssertFail();
    }
    if (*(u16 *)((u8 *)arg0 + 0x84) != 0) {
        *(u16 *)((u8 *)arg0 + 0x84) -= 1;
        ov41_0224A60C(*(u16 *)((u8 *)arg0 + 0x86), *(u8 *)(&ov41_0224C094 + (*(u16 *)((u8 *)arg0 + 0x84) ^ 1)), arg0);
        return;
    }
    if (gSystem.newKeys & 3) {
        var_r4 = 4;
    } else if (0x40 & gSystem.newKeys) {
        var_r4 = 1;
    } else if (0x80 & gSystem.newKeys) {
        var_r4 = 0;
    } else if (0x20 & gSystem.newKeys) {
        var_r4 = 2;
    } else if (gSystem.newKeys & 0x10) {
        var_r4 = 3;
    }
    if (var_r4 != 0xFF) {
        *(u16 *)((u8 *)arg0 + 0x86) = var_r4;
        *(u16 *)((u8 *)arg0 + 0x84) = 2U;
        ov41_0224A60C(*(u16 *)((u8 *)arg0 + 0x86), 0U, arg0);
    }
}
