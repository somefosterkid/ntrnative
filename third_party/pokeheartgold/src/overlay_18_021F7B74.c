#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "unk_02019BA4.h"


void ov18_021F7B90(void);

void ov18_021F7B74(PokedexAppData *pokedexApp) {
    GridInputHandler *inputHandler;

    inputHandler = pokedexApp->unk_1864;
    if (inputHandler != NULL) {
        GridInputHandler_Free(inputHandler);
        pokedexApp->unk_1864 = NULL;
    }
}

void ov18_021F7B90(void) {
}
