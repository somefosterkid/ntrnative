#include "global.h"

#include "overlay_112_pokewalker_app.h"
#include "unk_02005D10.h"

void ov112_021EA51C(void *, u32);

s32 ov112_021EC7B8(Ov112PokewalkerApp *);

void ov112_021EC7D0(Ov112PokewalkerApp *);

extern void *ov112_021FF0C4;

void ov112_021ECDA0(Ov112PokewalkerApp *param_1, s32 param_2);

void ov112_021ECDA0(Ov112PokewalkerApp *param_1, s32 param_2) {
    s32 iVar1;
    s8 uVar2;

    if (param_2 < 0) {
        if ((param_1->unk_1EC50 != 5) && (0 < (s8)param_1->unk_1EC54)) {
            param_1->unk_1EC54 = param_1->unk_1EC54 + -1;
            param_1->unk_1EC50 = 0;
            PlaySE(0x5dc);
        }
    } else if ((param_1->unk_1EC50 != 5) && (uVar2 = param_1->unk_1EC54, iVar1 = ov112_021EC7B8(param_1), (s32)(s8)uVar2 < iVar1 + -1)) {
        param_1->unk_1EC54 = param_1->unk_1EC54 + 1;
        param_1->unk_1EC50 = 0;
        PlaySE(0x5dc);
    }
    ov112_021EA51C(param_1, *(u32 *)(&ov112_021FF0C4 + param_1->unk_1EC50));
    ov112_021EC7D0(param_1);
    return;
}
