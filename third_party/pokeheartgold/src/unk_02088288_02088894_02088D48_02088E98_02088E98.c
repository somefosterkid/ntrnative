#include "global.h"

#include "item.h"
#include "sprite.h"
#include "sprite_system.h"
#include "system.h"
#include "unk_02005D10.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"

s32 sub_0208A71C(void *, s32);

s32 sub_0208A79C(void *);

s32 sub_0208A834(void *);

s32 sub_0208ADDC(void *);

void sub_0208AFA0(PokemonSummary *arg0, s32 arg1);

s32 sub_0208B044(PokemonSummary *, s32);

s32 sub_0208BB24(void *, u8, u8);

s32 sub_0208DB64(void *);

s32 sub_02088E98(PokemonSummary *arg0);

s32 sub_02088E98(PokemonSummary *arg0) {
    s32 temp_r0_2;
    if (gSystem.newKeys & 0x40) {
        if (sub_0208A71C(arg0, -1) == 1) {
            PlaySE(0x5DCU);
            sub_0208A79C(arg0);
        }
        return 6;
    }
    if (0x80 & gSystem.newKeys) {
        if (sub_0208A71C(arg0, 1) == 1) {
            PlaySE(0x5DCU);
            sub_0208A79C(arg0);
        }
        return 6;
    }
    if (gSystem.newKeys & 1) {
        sub_0208AFA0(arg0, 1);
        Sprite_SetDrawFlag(arg0->sprites[10], 0);
        if (arg0->moveSlot != arg0->swapMoveSlot) {
            PlaySE(0x5DDU);
            sub_0208A834(arg0);
            sub_0208BB24(arg0, arg0->moveSlot, arg0->swapMoveSlot);
            sub_0208DB64(arg0);
            sub_0208A79C(arg0);
        } else {
            PlaySE(0x5DCU);
        }
        return 5;
    }
    if (2 & gSystem.newKeys) {
        PlaySE(0x940U);
        sub_0208AFA0(arg0, 1);
        Sprite_SetDrawFlag(arg0->sprites[10], 0);
        return sub_0208B044(arg0, 5);
    }
    temp_r0_2 = sub_0208ADDC(arg0);
    if (temp_r0_2 == 4) {
        PlaySE(0x940U);
        sub_0208AFA0(arg0, 1);
        Sprite_SetDrawFlag(arg0->sprites[10], 0);
        return sub_0208B044(arg0, 5);
    }
    if ((temp_r0_2 != (-1U)) && ((*((u16 *)&arg0->mon.moves[temp_r0_2])) != 0)) {
        sub_0208AFA0(arg0, 1);
        Sprite_SetDrawFlag(arg0->sprites[10], 0);
        if (temp_r0_2 != arg0->swapMoveSlot) {
            arg0->moveSlot = temp_r0_2;
            PlaySE(0x5DDU);
            sub_0208A834(arg0);
            sub_0208BB24(arg0, arg0->moveSlot, arg0->swapMoveSlot);
            sub_0208DB64(arg0);
            sub_0208A79C(arg0);
        } else {
            PlaySE(0x5DCU);
        }
        return 5;
    }
    return 6;
}

s32 sub_0208AE48(void *);

s32 sub_0208AED4(void *arg0);

void sub_0208BBDC(PokemonSummary *arg0);

s32 sub_0208DBF0(void *);

s32 sub_02089028(PokemonSummary *arg0);

s32 sub_02089028(PokemonSummary *arg0) {
    s32 temp_r0_2;
    s32 temp_r7;
    u32 temp_r0;
    void *temp_r6;
    u32 keys = gSystem.newKeys;
    if (keys & 0x40) {
        if (sub_0208A71C(arg0, -1) == 1) {
            PlaySE(0x5DCU);
            sub_0208A79C(arg0);
        }
        return 8;
    }
    if (keys & 0x80) {
        if (sub_0208A71C(arg0, 1) == 1) {
            PlaySE(0x5DCU);
            sub_0208A79C(arg0);
        }
        return 8;
    }
    if (keys & 1) {
        PlaySE(0x5DDU);
        temp_r0 = arg0->moveSlot;
        if (temp_r0 == 4) {
            arg0->args->unk16 = 4;
            arg0->args->unk17 = 0;
            return 0x15;
        }
        if ((MoveIsHM(arg0->mon.moves[temp_r0]) == 1) && ((arg0->args->moveToLearn) != 0)) {
            thunk_Sprite_SetDrawFlag(arg0->sprites[18], 0);
            sub_0208DBF0(arg0);
            return 8;
        }
        return sub_0208AED4(arg0);
    }
    if (keys & 2) {
        PlaySE(0x940U);
        arg0->args->unk16 = 4;
        arg0->args->unk17 = 1;
        return sub_0208B044(arg0, 0x15);
    }
    temp_r0_2 = sub_0208AE48(arg0);
    switch (temp_r0_2) {
    case 4:
        if ((arg0->args->moveToLearn) != 0) {
            PlaySE(0x5DDU);
            arg0->moveSlot = 4;
            sub_0208A79C(arg0);
        }
        break;

    case -1:
        break;

    case 5:
        PlaySE(0x940U);
        arg0->moveSlot = 5;
        arg0->args->unk16 = 4;
        arg0->args->unk17 = 0;
        return sub_0208B044(arg0, 0x15);

    default:
        temp_r6 = arg0->mon.moves;
        temp_r7 = temp_r0_2 * 2;
        if ((MoveIsHM(*((u16 *)(temp_r6 + temp_r7))) == 1) && ((arg0->args->moveToLearn) != 0)) {
            PlaySE(0x5DDU);
            arg0->moveSlot = temp_r0_2;
            thunk_Sprite_SetDrawFlag(arg0->sprites[18], 0);
            sub_0208BBDC(arg0);
            sub_0208DBF0(arg0);
            return 8;
        }
        if ((*((u16 *)(temp_r6 + temp_r7))) != 0) {
            PlaySE(0x5DDU);
            arg0->moveSlot = temp_r0_2;
            sub_0208A79C(arg0);
            return sub_0208AED4(arg0);
        }
        break;
    }
    return 8;
}
