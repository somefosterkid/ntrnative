#include "global.h"

void ov112_021E7620(u8 *, u8 *);

void ov112_021E7CC8(u8 *);

void ov112_021ED25C(u8 *);

u32 ov112_021ED8DC(u8 *param_1);

u32 ov112_021ED8DC(u8 *param_1) {
    MIi_CpuClearFast(0, (void *)(param_1 + 0xaabc), 0x6c8);
    ov112_021E7620(param_1 + 0x9d70, param_1 + 0xb184);
    ov112_021E7CC8(param_1);
    ov112_021ED25C(param_1);
    return 0x1d;
}
