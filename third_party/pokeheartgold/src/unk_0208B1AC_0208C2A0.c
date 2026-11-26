#include "global.h"

#include "sprite.h"
#include "unk_0208B1AC.h"
#include "sprite_system.h"
#include "pokemon_summary_app.h"

u8 sub_0208ACDC(void *, u8);

#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/unk_0208B1AC_0208C0E8.c (tools/pokehg_abi.py)
void sub_0208C250(void *, u8, s32);
#else
s32 sub_0208C250(void *, s32, u8);
#endif

void sub_0208C2A0(PokemonSummary *arg0) {
    s32 v1;
    u16 v0;

    v0 = 0x36;
    do {
        Sprite_SetDrawFlag(arg0->sprites[v0], 0);
        v0 += 1;
    } while ((u32)v0 <= 0x42U);
    if (arg0->page == 2) {
        v1 = 0;
        do {
            if (v1 < (s32) arg0->ribbonCount) {
                Sprite_SetDrawFlag(arg0->sprites[v1 + 54], 1);
                sub_0208C250(arg0, sub_0208ACDC(arg0, (u8)v1), (u8)v1);
            }
            v1 = (s32)(u16)(v1 + 1);
        } while ((u32)v1 < 9U);
        Sprite_SetAnimCtrlSeq(arg0->sprites[66], 0);
    }
}

void sub_0208C320(PokemonSummary *arg0) {
    u16 v0;
    v0 = 0;
    do {
        if (((s32)(v0 + ((arg0->ribbonRow) * 9))) < ((s32)(arg0->ribbonCount))) {
            Sprite_SetDrawFlag(arg0->sprites[v0 + 54], 1);
            sub_0208C250(arg0, sub_0208ACDC(arg0, v0), v0);
        } else {
            Sprite_SetDrawFlag(arg0->sprites[v0 + 54], 0);
        }
        v0 = (u16)(v0 + 1);
    } while (((u32)v0) < 9U);
}

void sub_0208C380(PokemonSummary *arg0) {
    u8 v0;

    v0 = arg0->ribbonCursor;
    Sprite_SetPositionXY(arg0->sprites[63], (s16)((((s32)v0 % 3) << 5) + 0x20), (s16)((0x28 * ((s32)v0 / 3)) + 0x18));
}

void sub_0208C3C0(PokemonSummary *arg0) {
    if (Sprite_GetDrawFlag(arg0->sprites[66]) == 1) {
        Sprite_UpdateAnim(arg0->sprites[66], 0x1000);
    }
}
