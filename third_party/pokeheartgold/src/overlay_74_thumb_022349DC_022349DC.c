#include "global.h"

typedef struct {
    u16 gbaSpecies;
    u16 ndsSpecies;
} GBAToDSSpeciesStruct;

extern GBAToDSSpeciesStruct ov74_0223CC5C[144];

int TranslateAgbSpecies(int species);

int TranslateAgbSpecies(int species) {
    int i;

    if (species < 277) {
        return species;
    }

    for (i = 0; i < sizeof(ov74_0223CC5C) / sizeof(GBAToDSSpeciesStruct); i++) {
        if (ov74_0223CC5C[i].gbaSpecies == species) {
            return ov74_0223CC5C[i].ndsSpecies;
        }
    }

    return species;
}
