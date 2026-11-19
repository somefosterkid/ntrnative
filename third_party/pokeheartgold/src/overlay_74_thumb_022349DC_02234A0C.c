#include "global.h"

#include "pokemon.h"

u32 AGB_GetBoxMonData(void *, s32, void *);

int TranslateAgbSpecies(int species);

void ov74_02234A0C(void *arg0);

void ov74_02234A0C(void *arg0) {
    u32 species;

    species = TranslateAgbSpecies(AGB_GetBoxMonData(arg0, 0xB, 0));
    CalcLevelBySpeciesAndExp(species, AGB_GetBoxMonData(arg0, 0x19, 0));
}
