#include "global.h"

#include "pokewalker.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021ED590(u8 *param_1);

void ov112_021ED5DC(void *arg0);
s32 ov112_021ED610(void *arg0);
s32 ov112_021ED640(Ov112PokewalkerApp *arg0);

void ov112_021ED590(u8 *param_1) {
    u32 new_var;
    sub_02032674(*((POKEWALKER **)(param_1 + 0x1e440)), (u32 *)(param_1 + 0x1d758), (u32 *)(param_1 + 0x1d75c));
    new_var = *((u32 *)(param_1 + 0x10f0));
    if (!((*((u32 *)(param_1 + 0x10f0))) >= (*((u32 *)(param_1 + 0x1d758))))) {
        sub_02032650(*((POKEWALKER **)(param_1 + 0x1e440)), *((u32 *)(param_1 + 0x1d758)), (u32)(*((u16 *)(param_1 + 0x9d7a))));
    } else {
        sub_02032650(*((POKEWALKER **)(param_1 + 0x1e440)), new_var, (u32)(*((u16 *)(param_1 + 0x9d7a))));
    }
}

void ov112_021ED5DC(void *arg0) {
    sub_02032674(*(POKEWALKER **)((u8 *)arg0 + 0x1E440), arg0 + 0x1D758, arg0 + 0x1D75C);
    sub_02032650(*(POKEWALKER **)((u8 *)arg0 + 0x1E440), *(u32 *)((u8 *)arg0 + 0x1D758), (u32) * (u16 *)((u8 *)arg0 + 0x9D7A));
}

s32 ov112_021ED610(void *arg0) {
    u16 v0;

    if ((((u32)(*(u8 *)((u8 *)arg0 + 0x10E7) << 0x1D) >> 0x1F) == 1) && ((v0 = *(u16 *)((u8 *)arg0 + 0x9D44), (v0 == 0)) || ((u32)v0 > 0x1EDU))) {
        return 0;
    }
    return 1;
}

s32 ov112_021ED640(Ov112PokewalkerApp *arg0) {
    u16 v0;

    if ((((u32)(arg0->unk_AABC << 0x1A) >> 0x1F) == 1) && ((v0 = arg0->unk_AD00.unk_0, (v0 == 0)) || ((u32)v0 > 0x1EDU))) {
        return 0;
    }
    return 1;
}
