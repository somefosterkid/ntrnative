#include "global.h"

#include "system.h"
#include "unk_02005D10.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"

s32 sub_02089E30(PokemonSummary *, s32);

s32 sub_02089E98(void *);

s32 sub_0208A2C0(void *, s8);

s8 sub_0208A2E0(void *, s8);

s32 sub_0208ADCC(void *);

void sub_0208ADB8(PokemonSummary *, enum MenuInputState);

s32 sub_0208AEB4(void *);

s32 sub_0208AEC4(void *);

s32 sub_0208B044(PokemonSummary *, s32);

s32 sub_0208B0B0(PokemonSummary *, s32, s32);

s32 sub_02088B40(PokemonSummary *arg0);

s32 sub_02088B40(PokemonSummary *arg0) {
    s32 temp_r0_3;
    s32 temp_r0_4;
    s32 temp_r0_2;

    if (arg0->exitRequested == 1) {
        arg0->args->unk17 = 1;
        return 0x15;
    }
    if (gSystem.newAndRepeatedKeys & 0x20) {
        sub_02089E30(arg0, -1);
        return 2;
    }
    if (0x10 & gSystem.newAndRepeatedKeys) {
        sub_02089E30(arg0, 1);
        return 2;
    }
    if (gSystem.newAndRepeatedKeys & 0x40) {
        sub_0208A2C0(arg0, -1);
        return 0x13;
    }
    if (0x80 & gSystem.newAndRepeatedKeys) {
        sub_0208A2C0(arg0, 1);
        return 0x13;
    }
    if (2 & gSystem.newKeys) {
        PlaySE(0x940U);
        arg0->args->unk17 = 1;
        sub_0208ADB8(arg0, MENU_INPUT_STATE_BUTTONS);
        return sub_0208B044(arg0, 0x15);
    }
    if (1 & gSystem.newKeys) {
        switch (arg0->page) {
        case 1:
            PlaySE(0x69BU);
            arg0->moveSlot = 0;
            return 3;
        case 2:
            if (arg0->ribbonCount != 0) {
                PlaySE(0x5DDU);
                *((s8 *)&arg0->ribbonCursor) = 0;
                return 0xA;
            }
            break;
        }
    }
    if (arg0->page == 1) {
        temp_r0_2 = sub_0208ADCC(arg0);
        if ((temp_r0_2 != -1U) && (arg0->mon.moves[temp_r0_2] != 0)) {
            PlaySE(0x69BU);
            arg0->moveSlot = temp_r0_2;
            return 3;
        }
    }
    if (arg0->page == 2) {
        temp_r0_3 = sub_0208AEC4(arg0);
        if ((temp_r0_3 != -1) && (temp_r0_3 < 9) && (temp_r0_3 < (s32) arg0->ribbonCount)) {
            PlaySE(0x5DDU);
            *((s8 *)&arg0->ribbonCursor) = (s8)temp_r0_3;
            return 0xA;
        }
    }
    if (arg0->args->unk11 == 2) {
        temp_r0_4 = sub_0208AEB4(arg0);
        if (temp_r0_4 == 0) {
            if (sub_0208A2E0(arg0, -1) != -1) {
                PlaySE(0x5DDU);
                return sub_0208B0B0(arg0, 0, 0x14);
            }
            return 2;
        }
        if (temp_r0_4 == 1) {
            if (sub_0208A2E0(arg0, 1) != -1) {
                PlaySE(0x5DDU);
                return sub_0208B0B0(arg0, 1, 0x14);
            }
            return 2;
        }
    }
    return sub_02089E98(arg0);
}
