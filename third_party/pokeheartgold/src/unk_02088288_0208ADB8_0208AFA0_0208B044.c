#include "global.h"

#include "bg_window.h"
#include "unk_0201956C.h"
#include "pokemon_summary_app.h"

s32 sub_0208AFE8(PokemonSummary *summary, s32, s32, s32, s32, s32, s32, s32, s32);

s32 sub_0208B044(PokemonSummary *arg0, s32 arg1);

s32 sub_0208B044(PokemonSummary *arg0, s32 arg1) {
    return sub_0208AFE8(arg0, 6, 0x17, 0x14, 9, 4, 1, 0, arg1);
}

s32 sub_0208B068(PokemonSummary *arg0, s32 arg1);

s32 sub_0208B068(PokemonSummary *arg0, s32 arg1) {
    return sub_0208AFE8(arg0, 6, 1, 0x13, 0xF, 4, 1, 0, arg1);
}

s32 sub_0208B08C(PokemonSummary *arg0, s32 arg1);

s32 sub_0208B08C(PokemonSummary *arg0, s32 arg1) {
    return sub_0208AFE8(arg0, 6, 1, 0x11, 0xA, 2, 1, 0, arg1);
}

s32 sub_0208B0B0(PokemonSummary *arg0, s32 arg1, s32 arg2);

s32 sub_0208B0B0(PokemonSummary *arg0, s32 arg1, s32 arg2) {
    if (arg1 == 0) {
        return sub_0208AFE8(arg0, 6, 0x18, 5, 6, 3, 1, 0, arg2);
    }
    return sub_0208AFE8(arg0, 6, 0x18, 0xD, 6, 3, 1, 0, arg2);
}

s32 sub_0208B0F4(PokemonSummary *arg0, s32 arg1);

s32 sub_0208B0F4(PokemonSummary *arg0, s32 arg1) {
    return sub_0208AFE8(arg0, 5, 0x1A, 0x1D, 6, 4, 1, 0, arg1);
}

s8 sub_0208A2E0(void *, s8);

void sub_0208B118(PokemonSummary *arg0);

void sub_0208B118(PokemonSummary *arg0) {
    if (sub_0208A2E0(arg0, -1) != -1) {
        sub_020196E8(arg0->cellManager, 5, 0x18, 5);
    } else {
        FillBgTilemapRect(arg0->bgConfig, 6U, 1U, 0x18U, 5U, 6U, 3U, 0x10U);
        ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 6U);
    }
    if (sub_0208A2E0(arg0, 1) != -1) {
        sub_020196E8(arg0->cellManager, 6, 0x18, 0xD);
        return;
    }
    FillBgTilemapRect(arg0->bgConfig, 6U, 1U, 0x18U, 0xDU, 6U, 3U, 0x10U);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 6U);
}
