#include "global.h"

#include "pokewalker.h"

void ov112_021F3244(SaveData *, void *);

void ov112_021E7DC0(u8 *param_1);

void ov112_021E7DC0(u8 *param_1) {
    void *srcp;

    ov112_021F3244(*(void **)(param_1 + 0x20), param_1 + 0xf200);
    *(u32 *)(param_1 + 0xf1d0) = 1;
    *(u16 *)(param_1 + 0xf1d4) = GAME_VERSION;
    srcp = sub_02032728(*(POKEWALKER **)(param_1 + 0x1e440));
    MIi_CpuCopyFast(srcp, (void *)(param_1 + 0xf1d8), 0x28);
    return;
}
