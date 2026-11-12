// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/pokedex_skin_save.c, ported by tools/port_file.py.
#include "types.h"
#include "save/pokedex_skin_save.h"
#include "gfl/std.h"
#include "save/save_control.h"

// The downloaded Pokédex skin, extra save block 7: the characters and the palette of the Pokédex's cover. The ROM has
// no string for the file, so the name is a guess

u32 func_0200e8b0(void) {
    return sizeof(PokedexSkinSave);
}

void func_0200e8b8(void *block) {
    MI_CpuFill8(block, 0, func_0200e8b0());
}

void *func_0200e8cc(void *block) {
    PokedexSkinSave *skin = block;

    return skin->chars;
}

void *func_0200e8d0(void *block) {
    PokedexSkinSave *skin = block;

    return skin->palette;
}

BOOL func_0200e8d8(void *block) {
    PokedexSkinSave *skin = block;

    return skin->valid;
}

void func_0200e8e0(void *block, u32 valid) {
    PokedexSkinSave *skin = block;

    skin->valid = valid;
}
