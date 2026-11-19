#include "global.h"
#include "application/pokedex/pokedex_internal.h"
#include "sprite.h"
#include "sprite_system.h"

s32 ov18_021F61F8(void *param_1, s32 param_2, u16 *param_3, u32 param_4);

u8 ov18_021F6214(PokedexAppData *param_1, s32 param_2, u8 *param_3);

s32 ov18_021F61F8(void *param_1, s32 param_2, u16 *param_3, u32 param_4) {
    u32 uVar1;

    uVar1 = 0;
    while (uVar1 < param_4) {
        if (param_2 <= (s32)(u32)*param_3) {
            break;
        }
        uVar1 = uVar1 + 1;
        param_3 = param_3 + 1;
    }
    return uVar1 + 0xe;
}

u8 ov18_021F6214(PokedexAppData *param_1, s32 param_2, u8 *param_3) {
    u16 uVar1;

    uVar1 = ManagedSprite_GetActiveAnim(param_1->unk_0670[param_2]);
    return *(u8 *)(param_3 + (u32)uVar1 + -0xe);
}
