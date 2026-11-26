#include "global.h"

#include "pokewalker.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021ED590(Ov112PokewalkerApp *param_1) {
    u32 new_var;
    sub_02032674(param_1->pokewalker, &param_1->unk_1D758, &param_1->unk_1D75C);
    new_var = param_1->unk_10F0;
    if (!((param_1->unk_10F0) >= (param_1->unk_1D758))) {
        sub_02032650(param_1->pokewalker, param_1->unk_1D758, (u32)(param_1->unk_9D7A));
    } else {
        sub_02032650(param_1->pokewalker, new_var, (u32)(param_1->unk_9D7A));
    }
}

void ov112_021ED5DC(Ov112PokewalkerApp *arg0) {
    sub_02032674(arg0->pokewalker, &arg0->unk_1D758, &arg0->unk_1D75C);
    sub_02032650(arg0->pokewalker, arg0->unk_1D758, (u32) * &arg0->unk_9D7A);
}

s32 ov112_021ED610(Ov112PokewalkerApp *arg0) {
    u16 v0;

    if ((((u32)(arg0->unk_10E7 << 0x1D) >> 0x1F) == 1) && ((v0 = arg0->unk_9D44, (v0 == 0)) || ((u32)v0 > 0x1EDU))) {
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
