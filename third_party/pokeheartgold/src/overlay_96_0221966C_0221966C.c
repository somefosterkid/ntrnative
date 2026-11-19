#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_96_ov96_digit_work.h"

void ov96_02219A5C(Ov96DigitWork *work, s32 value);

void ov96_0221966C(Ov96DigitWork *work, u8 param_2, u16 param_3, u8 *param_4);

void ov96_0221966C(Ov96DigitWork *work, u8 param_2, u16 param_3, u8 *param_4) {
    u32 uVar1;
    s32 iVar2;
    s32 iVar3;

    if (work == NULL) {
        GF_AssertFail();
    }
    if (param_2 >= 4) {
        GF_AssertFail();
    }
    iVar3 = 0;
    iVar2 = 0;
    do {
        ManagedSprite_SetAnimNoRestart(work->sprites[(u8)param_4[1] + 10], iVar3 + 6);
        if ((iVar2 != 3) && (*param_4 != param_4[2])) {
            iVar3 = iVar2 + 1;
        }
        iVar2 = iVar2 + 1;
        param_4 = param_4 + 2;
    } while (iVar2 < 4);
    uVar1 = ov96_021E5F24(work->course);
    if ((param_2 == uVar1) && (param_3 != work->unk_C2)) {
        ov96_02219A5C(work, (u8)param_3);
        work->unk_C2 = param_3;
    }
    return;
}
