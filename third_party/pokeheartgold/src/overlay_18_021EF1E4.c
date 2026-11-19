#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"

void ov18_021F9648(Window *, MsgData *, u32, u32, u32, u32, u32, u32);

void ov18_021EF1E4(PokedexAppData *param_1, s32 param_2);

void ov18_021EF25C(PokedexAppData *param_1, s32 param_2);
void ov18_021EF298(PokedexAppData *param_1, s32 param_2);
void ov18_021EF2D4(PokedexAppData *param_1, s32 param_2);
void ov18_021EF310(PokedexAppData *param_1, s32 param_2);
void ov18_021EF34C(PokedexAppData *param_1, s32 param_2);

void ov18_021EF1E4(PokedexAppData *param_1, s32 param_2) {
    FillWindowPixelBuffer(&param_1->windows[param_2], 0);
    ov18_021F9648(&param_1->windows[param_2], param_1->msgData, 0x1a, 0x24, 0, 0, 0x20100, 2);
    return;
}

void ov18_021EF220(PokedexAppData *param_1, s32 param_2);

void ov18_021EF220(PokedexAppData *param_1, s32 param_2) {
    FillWindowPixelBuffer(&param_1->windows[param_2], 0);
    ov18_021F9648(&param_1->windows[param_2], param_1->msgData, 0x1b, 0x14, 0, 0, 0x20100, 2);
    return;
}

void ov18_021EF25C(PokedexAppData *param_1, s32 param_2) {
    FillWindowPixelBuffer(&param_1->windows[param_2], 0);
    ov18_021F9648(&param_1->windows[param_2], param_1->msgData, 0x1c, 0x14, 0, 0, 0x20100, 2);
    return;
}

void ov18_021EF298(PokedexAppData *param_1, s32 param_2) {
    FillWindowPixelBuffer(&param_1->windows[param_2], 0);
    ov18_021F9648(&param_1->windows[param_2], param_1->msgData, 0x1d, 0x14, 0, 0, 0x20100, 2);
    return;
}

void ov18_021EF2D4(PokedexAppData *param_1, s32 param_2) {
    FillWindowPixelBuffer(&param_1->windows[param_2], 0);
    ov18_021F9648(&param_1->windows[param_2], param_1->msgData, 0x1e, 0x14, 0, 0, 0x20100, 2);
    return;
}

void ov18_021EF310(PokedexAppData *param_1, s32 param_2) {
    FillWindowPixelBuffer(&param_1->windows[param_2], 0);
    ov18_021F9648(&param_1->windows[param_2], param_1->msgData, 0x1f, 0x14, 0, 0, 0x20100, 2);
    return;
}

void ov18_021EF34C(PokedexAppData *param_1, s32 param_2) {
    FillWindowPixelBuffer(&param_1->windows[param_2], 0);
    ov18_021F9648(&param_1->windows[param_2], param_1->msgData, 0x20, 0x18, 0, 0, 0x20100, 2);
    return;
}
