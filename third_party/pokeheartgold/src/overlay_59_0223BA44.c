#include "global.h"
#include "sprite.h"
#include "party.h"
#include "pm_string.h"
#include "pokemon.h"
#include "overlay_59_juice_feed_app.h"

void ov59_0223BA44(Ov59JuiceFeedApp *arg0);

void ov59_0223BA44(Ov59JuiceFeedApp *arg0) {
    Sprite *temp_r0;
    s32 var_r4;

    var_r4 = 0;
    do {
        temp_r0 = arg0->sprites[var_r4];
        if (temp_r0 != NULL) {
            Sprite_Delete(temp_r0);
        }
        var_r4 += 1;
    } while (var_r4 < 0x22);
}

void ov59_0223BA64(Ov59JuiceMon *arg0, Party *arg1, Pokemon *arg2, u8 arg3, enum HeapID arg4) {
    arg0->mon = arg2;
    arg0->slot = arg3;
    arg0->species = (u16) GetMonData(arg2, 5, NULL);
    arg0->gender = (u8) GetMonData(arg2, 0x6F, NULL);
    arg0->level = (u8) GetMonData(arg2, 0xA1, NULL);
    arg0->unk_09 = (u8) (GetMonBaseStat_HandleAlternateForm((s32) arg0->species, (s32) GetMonData(arg2, 0x70, NULL), 0x1C) ^ 1);
    arg0->name = String_New(0xCU, arg4);
    GetMonData(arg2, 0x77, arg0->name);
    CalcMonPokeathlonPerformance(arg0->mon, &arg0->today);
    Party_GetMonAprijuiceModifiers(arg1, &arg0->modifiers, (s32) arg0->slot);
}
