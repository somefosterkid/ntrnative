#include "global.h"

#include "pokemon.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021EE9E4(Pokemon *arg0, u32 arg1, Ov112PokewalkerMonRec *arg2, u16 arg3, s32 arg4);

void ov112_021EE9E4(Pokemon *mon, u32 arg1, Ov112PokewalkerMonRec *arg2, u16 nature, s32 gen_mode) {
    s8 ptr;
    u8 gender;

    gender = 0;
    ptr = (s8)((u32)(arg2->unk_D << 0x1B) >> 0x1B);
    if (((u32)(arg2->unk_D << 0x19) >> 0x1E) == 1) {
        gender = 0xFE;
    }
    CreateMon(mon, (s32) * (u16 *)arg2, (s32) * &arg2->unk_2[10], 0x20, 1, (s32)ChangePersonalityToNatureGenderAndAbility(arg1, arg2->unk_0, nature, gender, (u8)(u8)(1 & nature), gen_mode), 1, (s32)arg1);
    SetMonData(mon, 6, arg2->unk_2);
    SetMonData(mon, 0x36, &arg2->unk_2[2]);
    SetMonData(mon, 0x37, &arg2->unk_2[4]);
    SetMonData(mon, 0x38, &arg2->unk_2[6]);
    SetMonData(mon, 0x39, &arg2->unk_2[8]);
    SetMonData(mon, 0x70, &ptr);
}
