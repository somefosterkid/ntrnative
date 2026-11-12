// The PC build compiles src/ov131/p_sta_info.c through this file: an external definition of the game's plain
// `inline` function (see port/extern/src/ov119/pokemontrade_save.c). Without it the summary's first page aborts at -O0.
#include "../../../../src/ov131/p_sta_info.c"

extern __typeof__(PStaInfo_LoadSpeciesName) PStaInfo_LoadSpeciesName;
