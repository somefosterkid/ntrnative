#include "global.h"

#include "sprite.h"
#include "sprite_system.h"

void ov112_021E7548(u8 *, u8 *, u8 *, u8 *);

void ov112_021E7CC8(u8 *);

void ov112_021EA5A4(u8 *, u32);

void ov112_021ED25C(u8 *param_1);

u32 ov112_021ED4D4(u8 *param_1);

u32 ov112_021ED4D4(u8 *param_1) {
    MIi_CpuClearFast(0, (void *)(param_1 + 0x9dfc), 0xcc0);
    ov112_021E7548(param_1 + 0x9d44, param_1 + 0x9d70, param_1 + 0xaabc, param_1 + 0xb184);
    ov112_021E7CC8(param_1);
    ov112_021ED25C(param_1);
    ov112_021EA5A4(param_1, 2);
    ManagedSprite_SetAnimationFrame(*(ManagedSprite **)(param_1 + 0x1e550), 0);
    return 0x1b;
}
