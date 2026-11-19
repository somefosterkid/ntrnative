#include "global.h"

#include "constants/heap.h"

#include "overlay_59_juice_feed_app.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_types_def.h"

u8 sub_02032024(SaveApricornBox *arg0);

s32 ov58_022379C0(void *, Party *, s32);

extern s32 ov59_0223C99C[];

void ov59_0223BAE8(Ov59JuiceFeedApp *arg0) {
    Ov59JuiceMon *mon;
    s32 temp_r2;
    s32 i;

    mon = &arg0->mons[arg0->selected];
    ov58_022379C0(&arg0->juice, arg0->party, mon->slot);
    if (arg0->parent->unk_14 == 8) {
        sub_02032024(arg0->apricornBox);
    }
    Party_GetMonAprijuiceModifiers(arg0->party, &mon->modifiers, (s32)mon->slot);
    arg0->stars = mon->stars;
    CalcMonPokeathlonStars(&mon->stars, mon->mon, (s8 *)&mon->modifiers, arg0->heapId);
    i = 0;
    arg0->deltaCount = 0;
    do {
        temp_r2 = ov59_0223C99C[i] * 3;
        arg0->deltas[i] = (s8)(((u8)(7 & ((s32)mon->stars.stars >> temp_r2))) - ((u8)((((s32)arg0->stars.stars) >> temp_r2) & 7)));
        if (arg0->deltas[i] != 0) {
            arg0->deltaCount += 1;
        }
        i += 1;
    } while (i < 5);
    arg0->mode = 1;
}
