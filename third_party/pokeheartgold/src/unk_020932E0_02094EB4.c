#include "global.h"

#include "sprite.h"
#include "unk_020932E0.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_types_def.h"

void sub_02094EB4(UnkStruct_020932E0 *arg0) {
    s32 v1;
    s32 v2;
    UnkStruct_020932E0_SlotC *v0;

    v2 = 0;
    if ((s32)arg0->unk_D > 0) {
        v1 = 0;
        do {
            v0 = &arg0->unk_8D0[v2];
            if ((v0->unk_4 != -1) && (arg0->unk_F == v0->unk_8)) {
                Sprite_SetDrawFlag(v0->unk_0, 1);
                Sprite_SetOamMode(arg0->unk_198[arg0->unk_8D0[v2].unk_4].unk_0, GX_OAM_MODE_XLU);
            } else {
                Sprite_SetDrawFlag(v0->unk_0, 0);
            }
            v2 += 1;
            v1 += 0xC;
        } while (v2 < (s32)arg0->unk_D);
    }
}

void sub_02094F14(UnkStruct_020932E0 *param_1, s32 param_2, UnkStruct_020932E0_Info *param_3) {
    Pokemon *mon;
    u32 uVar1;

    mon = Party_GetMonByIndex(param_1->unk_4640, param_2);
    uVar1 = GetMonData(mon, 5, NULL);
    param_3->unk_0 = uVar1;
    uVar1 = GetMonData(mon, 0x4c, NULL);
    param_3->unk_8 = (s16)uVar1;
    uVar1 = GetMonData(mon, 0x70, NULL);
    param_3->unk_A = (s16)uVar1;
    uVar1 = GetMonData(mon, 6, NULL);
    param_3->unk_C = (s16)uVar1;
    param_3->unk_E = 0;
    return;
}
