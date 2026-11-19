#include "global.h"

#include "sprite.h"
#include "unk_02005D10.h"
#include "pokemon_summary_app.h"

u8 sub_0208ACDC(PokemonSummary *param_1, u8 param_2);

void sub_0208C320(PokemonSummary *);

void sub_0208C380(PokemonSummary *);

void sub_0208D7C4(PokemonSummary *);

void sub_0208AB58(PokemonSummary *param_1, s32 param_2);

void sub_0208AB58(PokemonSummary *param_1, s32 param_2) {
    u8 bVar1;
    u8 uVar4;

    bVar1 = param_1->ribbonCursor;
    uVar4 = param_1->ribbonRow;
    if (param_2 == 1) {
        if (bVar1 % 3 == 2) {
            return;
        }
        param_1->ribbonCursor += 1;
    } else if (param_2 == -1) {
        if (bVar1 % 3 == 0) {
            return;
        }
        param_1->ribbonCursor -= 1;
    } else if (param_2 == 3) {
        if (bVar1 >= 6) {
            if ((uVar4 + 1) * 9 < param_1->ribbonCount) {
                param_1->ribbonRow += 1;
                param_1->ribbonCursor = param_1->ribbonCursor % 3;
            }
        } else {
            param_1->ribbonCursor = bVar1 + 3;
        }
    } else if (param_2 == -3) {
        if (bVar1 <= 2) {
            if (uVar4 != 0) {
                param_1->ribbonRow -= 1;
                param_1->ribbonCursor = param_1->ribbonCursor % 3 + 6;
            }
        } else {
            param_1->ribbonCursor = bVar1 - 3;
        }
    } else if (param_2 == 9) {
        if ((uVar4 + 1) * 9 < param_1->ribbonCount) {
            param_1->ribbonRow += 1;
        }
    } else if (param_2 == -9) {
        if (uVar4 != 0) {
            param_1->ribbonRow -= 1;
        }
    }
    if ((bVar1 != param_1->ribbonCursor || uVar4 != param_1->ribbonRow) && param_2 != 9 && param_2 != -9) {
        PlaySE(0x5dc);
    }
    param_1->ribbonId = sub_0208ACDC(param_1, param_1->ribbonCursor);
    sub_0208C380(param_1);
    if (uVar4 != param_1->ribbonRow) {
        sub_0208C320(param_1);
    }
    sub_0208D7C4(param_1);
    if (param_1->ribbonRow != 0) {
        Sprite_SetDrawFlag(param_1->sprites[64], 1);
    } else {
        Sprite_SetDrawFlag(param_1->sprites[64], 0);
    }
    if ((param_1->ribbonRow + 1) * 9 < param_1->ribbonCount) {
        Sprite_SetDrawFlag(param_1->sprites[65], 1);
    } else {
        Sprite_SetDrawFlag(param_1->sprites[65], 0);
    }
}
