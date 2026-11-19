#include "global.h"

#include "pokemon_icon_idx.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokewalker.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021E9464(BoxPokemon *arg0);

extern u8 ov112_021F397E;
u32 ov112_021E9480(u32 arg0, s32 arg1, u16 arg2);
extern u32 ov112_021F4138;
void ov112_021E95A0(Ov112PokewalkerApp *arg0);

s32 ov112_021E9464(BoxPokemon *boxMon) {
    return Boxmon_GetIconNaix(boxMon) - 7;
}

s32 ov112_021E9470(u32 arg0);

s32 ov112_021E9470(u32 species) {
    return GetMonIconNaixEx(species, 0, 0U) - 7;
}

u32 ov112_021E9480(u32 species, s32 arg1, u16 form) {
    s32 v1;
    u8 v0;

    v1 = arg1;
    v0 = sub_02070438(species, form);
    switch ((s32)species) {
    case 0x19C:
        return v0 + 0x26E;
    case 0x19D:
        return v0 + 0x271;
    case 0x1A6:
        return v0 + 0x274;
    case 0x1A7:
        return v0 + 0x276;
    case 0x1A5:
        return v0 + 0x278;
    case 0x1ED:
        return v0 + 0x27A;
    case 0x15F:
        return v0 + 0x26A;
    case 0x182:
        return v0 + 0x24A;
    case 0xC9:
        return v0 + 0x24E;
    case 0x1EC:
        return v0 + 0x28C;
    case 0x1DF:
        return v0 + 0x28E;
    case 0x1E7:
        return v0 + 0x294;
    case 0xAC:
        return v0 + 0x296;
    default:
        if (v1 != 1) {
            v1 = 0;
        }
        return *(u16 *)((v1 * 2) + (&ov112_021F397E + (species * 4)));
    }
}

void ov112_021E95A0(Ov112PokewalkerApp *arg0) {
    u32 a1;
    u32 a2;
    unsigned int new_var;
    s32 courseNo;
    u32 *v0;
    u32 v1;
    sub_02032674(arg0->pokewalker, &a1, &a2);
    new_var = 0U;
    if (Pokedex_GetNatDexFlag(Save_Pokedex_Get(arg0->saveData)) != 0) {
        v1 = 0x14;
    } else {
        v1 = 8;
    }
    courseNo = 0;
    if (v1 > new_var) {
        v0 = &ov112_021F4138;
loop_5:
        if ((((u32)(*v0)) <= a2) && (Pokewalker_CourseIsUnlocked(arg0->pokewalker, courseNo) == 0)) {
            Pokewalker_UnlockCourse(arg0->pokewalker, courseNo);
            arg0->unk_1EC78 = courseNo;
            return;
        }

        courseNo += 1;
        v0 += 48;
        if (((u32)courseNo) >= v1) {
        } else {
            goto loop_5;
        }
    }
}
