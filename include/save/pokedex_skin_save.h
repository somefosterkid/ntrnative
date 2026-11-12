// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/pokedex_skin_save.h: the lines in use
// so far.
#ifndef BWPORT_SAVE_POKEDEX_SKIN_SAVE_H
#define BWPORT_SAVE_POKEDEX_SKIN_SAVE_H

#include "types.h"

typedef struct {
    u8 chars[0x6000];
    u8 palette[0x200];
    BOOL valid;
} PokedexSkinSave; // size: 0x6204

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_0200e8b8(void *block);

#endif // BWPORT_SAVE_POKEDEX_SKIN_SAVE_H
