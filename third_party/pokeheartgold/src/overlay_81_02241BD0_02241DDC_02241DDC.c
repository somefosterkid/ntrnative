#include "global.h"

#include "party.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "overlay_81_battle_factory_app.h"

s32 ov81_02240F48(void *, s32, Pokemon *, s32, s32, s32);

void ov81_02241DDC(BattleFactoryApp *app, s32 arg1);

void ov81_02241DDC(BattleFactoryApp *app, s32 arg1) {
    s32 sp14;
    unsigned int new_var2;
    s32 sp10;
    s32 spC;
    u16 *sp18;
    s32 sp8;
    u32 temp_r1;
    u32 var_r4;
    BattleFactoryApp *new_var;
    Pokepic **var_r7;

    sp8 = arg1;
    temp_r1 = app->numMonsToFade;
    if (temp_r1 == 3) {
        sp10 = 0x30;
        spC = 0x50;
    } else {
        sp10 = 0x48;
        spC = 0x70;
    }
    var_r4 = 0;
    new_var = app;
    new_var2 = 0U;
    if (temp_r1 > new_var2) {
        sp14 = 0;
        var_r7 = (Pokepic **)(sp18 = (u16 *)app);
        do {
            ov81_02240F48(app, (u8)var_r4, Party_GetMonByIndex(new_var->personalParty, (s32)(*((u16 *)(((u8 *)sp18) + offsetof(BattleFactoryApp, selectedIndices))))), sp10 + sp14, 0x90, sp8);
            Pokepic_SetAttr(*((Pokepic **)(((u8 *)var_r7) + offsetof(BattleFactoryApp, monSprites))), 6, 1);
            var_r4 += 1;
            sp14 += spC;
            var_r7 += 1;
            sp18 += 1;
        } while (((u32)var_r4) < ((u32)app->numMonsToFade));
    }
}
