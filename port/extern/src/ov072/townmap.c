// The PC build compiles src/ov072/townmap.c through this file: an external definition of the game's plain `inline`
// function (see port/extern/src/ov119/pokemontrade_save.c).
#include "../../../../src/ov072/townmap.c"

extern __typeof__(TownMapMap_SetLimits) TownMapMap_SetLimits;
