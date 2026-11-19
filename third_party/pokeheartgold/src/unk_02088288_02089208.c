#include "global.h"

#include "sprite.h"
#include "system.h"
#include "unk_02005D10.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"

s32 sub_0208AE88(void *);

s32 sub_0208AF08(void *, s32);

s32 sub_0208B044(PokemonSummary *, s32);

s32 sub_0208B068(PokemonSummary *, s32);

s32 sub_02089208(PokemonSummary *arg0);

s32 sub_0208A9C4();
s32 sub_020892F4(void);
s32 sub_0208AA9C();
s32 sub_02089308(void);

s32 sub_02089208(PokemonSummary *arg0) {
    s32 v0;

    if (1 & gSystem.newKeys) {
        PlaySE(0x5DDU);
        arg0->args->unk16 = (s8)arg0->moveSlot;
        arg0->args->unk17 = 0;
        return sub_0208B068(arg0, 0x15);
    }
    if (2 & gSystem.newKeys) {
        PlaySE(0x940U);
        Sprite_SetAnimCtrlSeq(arg0->sprites[9], 0);
        Sprite_SetDrawFlag(arg0->sprites[10], 0);
        sub_0208AF08(arg0, 0);
        return sub_0208B044(arg0, 8);
    }
    v0 = sub_0208AE88(arg0);
    switch (v0) {
    case 0:
        PlaySE(0x5DDU);
        arg0->args->unk16 = (s8)arg0->moveSlot;
        arg0->args->unk17 = 0;
        return sub_0208B068(arg0, 0x15);
    case 1:
        PlaySE(0x940U);
        Sprite_SetAnimCtrlSeq(arg0->sprites[9], 0);
        Sprite_SetDrawFlag(arg0->sprites[10], 0);
        sub_0208AF08(arg0, 0);
        return sub_0208B044(arg0, 8);
    default:
    case -1:
        return 9;
    }
}

s32 sub_020892F4(void) {
    if (sub_0208A9C4() == 1) {
        return 0xC;
    }
    return 0xA;
}

s32 sub_02089308(void) {
    if (sub_0208AA9C() == 1) {
        return 2;
    }
    return 0xB;
}
