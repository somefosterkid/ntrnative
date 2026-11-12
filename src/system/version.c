// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/version.c: the game's version and region,
// a Pokémon's origin game and where the cartridge was sold (the ROM has no string for the file, so the name is a
// guess). Black's link has it between pass_power.c and vm.c, whose range held its function.
#include "types.h"
#include "constants/version.h"
#include "system/version.h"

u32 getGameVersion(void) {
    return GAME_VERSION;
}

const u8 game_version = GAME_VERSION;
const u8 region = 2;
