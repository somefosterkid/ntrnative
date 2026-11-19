#include "global.h"
struct PokemonSummary;

#include "sprite.h"
#include "sprite_system.h"
#include "pokemon_summary_app.h"
#include "pokemon_summary_args.h"

extern s32 _021105B0;

void sub_0208B400(struct PokemonSummary *arg0);

void sub_0208B400(PokemonSummary *arg0) {
    s32 temp_r3;
    PokemonSummaryArgs *temp_r1;
    unsigned char new_var;
    temp_r1 = arg0->args;
    if (temp_r1->unk11 != 2) {
        temp_r3 = temp_r1->partySlot * 2;
        new_var = *((u8 *)(((u8 *)&_021105B0) + temp_r3));
        Sprite_SetPositionXY(arg0->sprites[9], (s16)new_var, (s16)(*((u8 *)((((u8 *)&_021105B0) + 1) + temp_r3))));
        Sprite_SetAnimCtrlSeq(arg0->sprites[9], 2);
        return;
    }
    Sprite_SetDrawFlag(arg0->sprites[9], 0);
}
