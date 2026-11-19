#include "global.h"

#include "bg_window.h"
#include "application/pokedex/pokedex_internal.h"

void ov18_021F6AB0(PokedexAppData *param_1, s32 param_2, s32 param_3);

void ov18_021F6AB0(PokedexAppData *param_1, s32 param_2, s32 param_3) {
    fx32 denom;
    MtxFx22 MStack_1c;

    denom = FX_Div(0x100000, param_3 << 0xc);
    MStack_1c._00 = FX_Inv(denom);
    MStack_1c._01 = 0;
    MStack_1c._10 = 0;
    MStack_1c._11 = MStack_1c._00;
    Bg_SetTextDimAndAffineParams(param_1->bgConfig, 7, BG_POS_OP_SET_ROT, 0x38, &MStack_1c, 0x80, 0x80);
    Bg_SetTextDimAndAffineParams(param_1->bgConfig, 7, BG_POS_OP_SET_XSCALE, 0x10 - param_2, &MStack_1c, 0x80, 0x80);
    return;
}
