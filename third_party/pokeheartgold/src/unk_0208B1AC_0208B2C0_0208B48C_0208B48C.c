#include "global.h"

#include "sprite_system.h"
#include "pokemon_summary_app.h"

extern s32 _02104C68;

void sub_0208B48C(PokemonSummary *arg0);

void sub_0208B48C(PokemonSummary *arg0) {
    s32 var_r3;
    u32 temp_r3;

    temp_r3 = arg0->mon.pokeball;
    if (temp_r3 == 0) {
        var_r3 = 0x19;
    } else {
        var_r3 = temp_r3 + 0x18;
    }
    SpriteSystem_ReplaceCharResObj(arg0->spriteSystem, arg0->spriteManager, NARC_a_1_6_2, var_r3, 0, 0);
    SpriteSystem_ReplacePlttResObj(arg0->spriteSystem, arg0->spriteManager, NARC_a_1_6_2, *(u8 *)(((u8 *)&_02104C68) + (arg0->mon.pokeball)) + 0x31, 0, 6);
}
