// Burmy's form after a battle, from the terrain it was fought on. The name is descriptive: the ROM has no string for
// this file, which the order of overlay 12's .rodata separates from event_battle.c (its 140-byte table comes before
// that file's smaller ones, which one file's size-sorted data can't do). Function name from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "field/burmy_form.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "pml/poke_party.h"
#include "save/pokedex_savedata.h"
#include "system/game_data.h"

// Burmy's form for each terrain
typedef struct {
    u16 terrain;
    u16 form;
} BurmyForm;

// clang-format off
static const BurmyForm BURMY_FORMS[35] = {
#include "asset/burmy_form_BURMY_FORMS.inc"
};

// clang-format on

void burmyTransform(GameData *gameData, PartyPkm *pkm, u32 terrain) {
    PokeDexSave *pokedex = GameData_GetPokedex(gameData);
    u32 species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
    int i;

    for (i = 0; i < 35; i++) {
        if (terrain == BURMY_FORMS[i].terrain) {
            u32 form = BURMY_FORMS[i].form;

            if (species == SPECIES_BURMY) {
                PokeParty_ChangeForme(pkm, form);
                PokeDex_RegistPkm(pokedex, pkm);
            }
            return;
        }
    }
}
